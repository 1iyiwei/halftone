#!/bin/tcsh -f

set halftone_dir = .;
set rainbow_dir = ../RainbowNoise;
set output_dir = data;
set huge_dir = data/huge;

# color error diffusion for constant image, i.e. uniform sampling
if(0) then

set image_size = 128;

set color_r = 16;
set color_g = 16;
set color_b = 16;

set input_root_name = r${color_r}g${color_g}b${color_b};
# set diffuser_type = floydsteinberg;
set diffuser_type = varcoeff;
set diffuser_type = zhou;

set scanner_type = serpentine;

set palette_root_name = palette;
echo "0 0 0   1 0 0   0 1 0   0 0 1" > ${output_dir}/${palette_root_name}.txt;

set palette_file_name = ${output_dir}/${palette_root_name}.txt;
#set palette_file_name = "null";

set output_root_name = ${input_root_name}_${diffuser_type}_${scanner_type};

convert -size ${image_size}x${image_size} xc:none -fill "rgb(${color_r},${color_g},${color_b})" -draw "rectangle 0,0,${image_size},${image_size}" -compress none ${output_dir}/${input_root_name}.ppm;
convert ${output_dir}/${input_root_name}.ppm -compress none ${output_dir}/${input_root_name}.ppm;

./src/Main/Release/ErrorDiffuse ${output_dir}/${input_root_name}.ppm ${output_dir}/${output_root_name}.ppm ${diffuser_type} ${scanner_type} ${palette_file_name};

endif

# color error diffusion for constant float image
if(0) then

set image_size = 128;
set intensity = 0625; # fraction part

set input_root_name = g${intensity};
# set diffuser_type = floydsteinberg;
set diffuser_type = varcoeff;
set diffuser_type = zhou;

set scanner_type = serpentine;

set palette_root_name = palette;
echo "0 0 0   1 0 0   0 1 0   0 0 1" > ${output_dir}/${palette_root_name}.txt;

set palette_file_name = ${output_dir}/${palette_root_name}.txt;
#set palette_file_name = "null";

set output_root_name = ${input_root_name}_${diffuser_type}_${scanner_type};

./src/Main/Release/ErrorDiffuse -${image_size}.${intensity} ${output_dir}/${output_root_name}.ppm ${diffuser_type} ${scanner_type} ${palette_file_name};

endif

# void and cluster, spectrum
if(0) then

set dimension = 2;
set grid_size = 256; # 128 or 256

set num_classes = 2;
set num_samples = "400 400"; # friendly for 1in paper image
set num_samples_array = (400 400);
set total_num_samples = 800;
set num_samples_text = 800;

if(1) then
set num_classes = 3;
set num_samples = "400 400 400"; # friendly for 1in paper image
set num_samples_array = (400 400 400);
set total_num_samples = 1200;
set num_samples_text = 1200;
endif

if(0) then
set num_classes = 2;
set num_samples = "150 1050"; # 1:7 ratio for worst case hump
set num_samples_array = (150 1050);
set total_num_samples = 1200;
set num_samples_text = 1200;
endif

set relative_splat_size = 1.0;
set init_swap_size = 32;
set min_num_iterations = 200;

set background_color = "0 0 0";

set palette_root_name = palette;
echo "255 0 0   0 255 0   0 0 255" > ${output_dir}/${palette_root_name}.txt;

set palette_file_name = ${output_dir}/${palette_root_name}.txt;
#set palette_file_name = "null";

set num_runs = 10;

# sft
set sft_freq = 256;
@ sft_size = ${sft_freq} * 2;
set sft_scale = ${sft_size};

set domain_size = "1 1";
set dot_radius = 0.0025;

foreach method (whiterandom) #(discrete gridjitter whiterandom)

if(${method} == gridjitter) then
set trough_candidate_size = ${init_swap_size};
set trough_jitter_radius = 5;
else if(${method} == whiterandom) then
set trough_candidate_size = ${grid_size};
set trough_jitter_radius = "";
else
set trough_candidate_size = "";
set trough_jitter_radius = "";
endif

foreach weighting_scheme (fair)

set output_base_name = voidcluster_${method}_${weighting_scheme}_${grid_size}g_${num_classes}c_${num_samples_text}s;

# generation
set which_run = 0;
while(${which_run} < ${num_runs})

set output_root_name = ${output_base_name}_${which_run};

set sample_root_name = ${output_root_name}_samples;

${halftone_dir}/src/Main/Release/VoidCluster ${method} ${grid_size} ${num_classes} ${num_samples} ${weighting_scheme} ${relative_splat_size} ${init_swap_size} ${trough_candidate_size} ${trough_jitter_radius} ${min_num_iterations} "cout" ${output_dir}/${palette_root_name}.txt > ${huge_dir}/${sample_root_name}.txt;

sleep 1;

@ which_run = ${which_run} + 1;
end # while 

# spectrum
foreach id (0 'all')

set sft_base_name = ${output_base_name}_${id};

set all_pfm_files = "";
set which_run = 0;
while(${which_run} < ${num_runs})

set output_root_name = ${output_base_name}_${which_run};
set sft_root_name = ${sft_base_name}_${which_run};

set input_sample_root_name = ${output_root_name}_samples;
set output_sample_root_name = ${sft_root_name}_samples;

if(${id} == 'all') then
set ids = "0 1 2";
else
set ids = "${id}";
@ id_plus_one = ${id} + 1;
endif

${rainbow_dir}/src/Main/Release/SampleSelector ${huge_dir}/${input_sample_root_name}.txt ${dimension} ${ids} > ${huge_dir}/${output_sample_root_name}_selected.txt
${rainbow_dir}/src/Sain/Release/SFT ${huge_dir}/${output_sample_root_name}_selected.txt ${huge_dir}/${output_sample_root_name}.pfm ${dimension} 1 1 ${sft_size} ${sft_scale}

set all_pfm_files = "$all_pfm_files ${huge_dir}/${output_sample_root_name}.pfm";

@ which_run = ${which_run} + 1;
end # while

echo ${all_pfm_files} > ${huge_dir}/all_pfm_files_list.txt;
${rainbow_dir}/src/Sain/Release/RadialPlot ${huge_dir}/all_pfm_files_list.txt ${output_dir}/${sft_base_name}_average${num_runs}.pfm ${output_dir}/${sft_base_name}_average${num_runs}_mean.txt ${output_dir}/${sft_base_name}_average${num_runs}_variance.txt
rm ${huge_dir}/all_pfm_files_list.txt;

${rainbow_dir}/src/Sain/Release/PFM2PPM ${output_dir}/${sft_base_name}_average${num_runs}.pfm ${output_dir}/${sft_base_name}_average${num_runs}.ppm 1 1 1

echo ${output_dir}/${sft_base_name}_average${num_runs}.ppm

if(${id} == 'all') then
set num_class_samples = ${total_num_samples};
else
set num_class_samples = $num_samples_array[$id_plus_one];
endif

${rainbow_dir}/src/Scripts/txt2dat ${output_dir}/${sft_base_name}_average${num_runs}_mean.txt ${output_dir}/${sft_base_name}_average${num_runs}_variance.txt ${num_class_samples} ${output_dir}/${sft_base_name}_average${num_runs}_mean.dat ${output_dir}/${sft_base_name}_average${num_runs}_variance.dat 1

# mean
echo set terminal postscript eps >> foo.gp
echo set output \'${output_dir}/${sft_base_name}_average${num_runs}_mean.eps\' >> foo.gp
echo set size 0.5, 0.675 >> foo.gp
echo set title \"\" >> foo.gp
echo set xlabel \"frequency\" >> foo.gp
echo set ylabel \"power\" >> foo.gp
echo set key off >> foo.gp
echo set autoscale y >> foo.gp
echo plot "[1:${sft_freq}]" \"${output_dir}/${sft_base_name}_average${num_runs}_mean.dat\" with line linetype rgb \"red\" >> foo.gp

wgnuplot foo.gp
rm foo.gp

set output_name = ${sft_base_name}_average${num_runs}_mean;
epstopdf ${output_dir}/${output_name}.eps --outfile ${output_dir}/${output_name}.pdf;
echo ${output_dir}/${output_name}.pdf;

# variance
echo set terminal postscript eps >> foo.gp
echo set output \'${output_dir}/${sft_base_name}_average${num_runs}_variance.eps\' >> foo.gp
echo set size 0.5, 0.675 >> foo.gp
echo set title \"\" >> foo.gp
echo set xlabel \"frequency\" >> foo.gp
echo set ylabel \"anisotropy\" >> foo.gp
echo set key off >> foo.gp
#echo set autoscale y >> foo.gp
echo plot "[1:${sft_freq}] [-20:20]" \"${output_dir}/${sft_base_name}_average${num_runs}_variance.dat\" with line linetype rgb \"blue\" >> foo.gp

wgnuplot foo.gp
rm foo.gp

set output_name = ${sft_base_name}_average${num_runs}_variance;
epstopdf ${output_dir}/${output_name}.eps --outfile ${output_dir}/${output_name}.pdf;
echo ${output_dir}/${output_name}.pdf;

end # foreach id

end # foreach weighting_scheme
end # foreach method

endif

# multi-class PD, try to match 3x400 samples
if(0) then

set dimension = 2;
set num_classes = 3;
set priority = "0 0 0";
set r_values = "0.036 0.036 0.036";
set r_text = 400;

set dot_radius = 0.005;

set k_value = 0.67;
set domain_size = "1 1";

set exe_name = DartThrowing;
set output_name = rainbow;

set output_root_name = ${output_name}_${dimension}d_${num_classes}c_${r_text};

${rainbow_dir}/src/Main/Release/${exe_name} $dimension $num_classes $priority $r_values $k_value $domain_size > ${output_dir}/${output_root_name}.txt

if(1) then
${rainbow_dir}/src/Main/Release/DrawSamples ${output_dir}/${output_root_name}.txt ${output_dir}/${output_root_name}.fig 2 ${dot_radius} 1 ${domain_size}
doc/fig2eps -f ${output_dir}/${output_root_name}.fig
epstopdf ${output_dir}/${output_root_name}.eps --outfile ${output_dir}/${output_root_name}.pdf
endif

echo ${output_root_name}
endif

# multi-class PD, spectrum
if(0) then

set dimension = 2;
set num_classes = 3;
set priority = "0 0 0";
set r_values = "0.036 0.036 0.036";
set num_samples_array = (399 399 399); # computed from r_values above
set total_num_samples = 1197;
set r_text = 400;

set k_value = 0.67;
set domain_size = "1 1";

set exe_name = DartThrowing;
set output_name = rainbow;

set output_base_name = ${output_name}_${dimension}d_${num_classes}c_${r_text};

set num_runs = 10;

# sft
set sft_freq = 256;
@ sft_size = ${sft_freq} * 2;
set sft_scale = ${sft_size};

# generation
set which_run = 0;
while(${which_run} < ${num_runs})

set output_root_name = ${output_base_name}_${which_run};

set sample_root_name = ${output_root_name}_samples;

${rainbow_dir}/src/Main/Release/${exe_name} $dimension $num_classes $priority $r_values $k_value $domain_size > ${huge_dir}/${sample_root_name}.txt

sleep 1;
@ which_run = ${which_run} + 1;
end

# spectrum
foreach id (0 'all')

set sft_base_name = ${output_base_name}_${id};

set all_pfm_files = "";
set which_run = 0;
while(${which_run} < ${num_runs})

set output_root_name = ${output_base_name}_${which_run};
set sft_root_name = ${sft_base_name}_${which_run};

set input_sample_root_name = ${output_root_name}_samples;
set output_sample_root_name = ${sft_root_name}_samples;

if(${id} == 'all') then
set ids = "0 1 2";
else
set ids = "${id}";
@ id_plus_one = ${id} + 1;
endif

${rainbow_dir}/src/Main/Release/SampleSelector ${huge_dir}/${input_sample_root_name}.txt ${dimension} ${ids} > ${huge_dir}/${output_sample_root_name}_selected.txt
${rainbow_dir}/src/Sain/Release/SFT ${huge_dir}/${output_sample_root_name}_selected.txt ${huge_dir}/${output_sample_root_name}.pfm ${dimension} 1 1 ${sft_size} ${sft_scale}

set all_pfm_files = "$all_pfm_files ${huge_dir}/${output_sample_root_name}.pfm";

@ which_run = ${which_run} + 1;
end # while

echo ${all_pfm_files} > ${huge_dir}/all_pfm_files_list.txt;
${rainbow_dir}/src/Sain/Release/RadialPlot ${huge_dir}/all_pfm_files_list.txt ${output_dir}/${sft_base_name}_average${num_runs}.pfm ${output_dir}/${sft_base_name}_average${num_runs}_mean.txt ${output_dir}/${sft_base_name}_average${num_runs}_variance.txt
rm ${huge_dir}/all_pfm_files_list.txt;

${rainbow_dir}/src/Sain/Release/PFM2PPM ${output_dir}/${sft_base_name}_average${num_runs}.pfm ${output_dir}/${sft_base_name}_average${num_runs}.ppm 1 1 1

echo ${output_dir}/${sft_base_name}_average${num_runs}.ppm

if(${id} == 'all') then
set num_class_samples = ${total_num_samples};
else
set num_class_samples = $num_samples_array[$id_plus_one];
endif

${rainbow_dir}/src/Scripts/txt2dat ${output_dir}/${sft_base_name}_average${num_runs}_mean.txt ${output_dir}/${sft_base_name}_average${num_runs}_variance.txt ${num_class_samples} ${output_dir}/${sft_base_name}_average${num_runs}_mean.dat ${output_dir}/${sft_base_name}_average${num_runs}_variance.dat 1

# mean
echo set terminal postscript eps >> foo.gp
echo set output \'${output_dir}/${sft_base_name}_average${num_runs}_mean.eps\' >> foo.gp
echo set size 0.5, 0.675 >> foo.gp
echo set title \"\" >> foo.gp
echo set xlabel \"frequency\" >> foo.gp
echo set ylabel \"power\" >> foo.gp
echo set key off >> foo.gp
echo set autoscale y >> foo.gp
echo plot "[1:${sft_freq}]" \"${output_dir}/${sft_base_name}_average${num_runs}_mean.dat\" with line linetype rgb \"red\" >> foo.gp

wgnuplot foo.gp
rm foo.gp

set output_name = ${sft_base_name}_average${num_runs}_mean;
epstopdf ${output_dir}/${output_name}.eps --outfile ${output_dir}/${output_name}.pdf;
echo ${output_dir}/${output_name}.pdf;

# variance
echo set terminal postscript eps >> foo.gp
echo set output \'${output_dir}/${sft_base_name}_average${num_runs}_variance.eps\' >> foo.gp
echo set size 0.5, 0.675 >> foo.gp
echo set title \"\" >> foo.gp
echo set xlabel \"frequency\" >> foo.gp
echo set ylabel \"anisotropy\" >> foo.gp
echo set key off >> foo.gp
#echo set autoscale y >> foo.gp
echo plot "[1:${sft_freq}] [-20:20]" \"${output_dir}/${sft_base_name}_average${num_runs}_variance.dat\" with line linetype rgb \"blue\" >> foo.gp

wgnuplot foo.gp
rm foo.gp

set output_name = ${sft_base_name}_average${num_runs}_variance;
epstopdf ${output_dir}/${output_name}.eps --outfile ${output_dir}/${output_name}.pdf;
echo ${output_dir}/${output_name}.pdf;

end # foreach id

endif
