#!/bin/tcsh -f

if ($#argv < 1) then
 echo "please specify the output directory"
 exit(1)
endif

set output_dir = $argv[1]

mkdir -p ${output_dir}

set exe_dir = ./bin/Release/

if(1) then

set input_root_name = ramp;
set region_height = 512;
set region_width = 2;
set gray_level_min = 0;
set gray_level_max = 255;

set image_height = ${region_height};
@ image_width = 1 + ${gray_level_max} - ${gray_level_min};
@ image_width = ${image_width} * ${region_width};

set stripe_root_name = ${input_root_name}_stripe;

magick -size ${image_width}x${image_height} xc:none -fill "rgb(0,0,0)" -draw "rectangle 0,0,${image_width},${image_height}" -compress none ${output_dir}/${input_root_name}.ppm

set draw_string = "";

set token = \";

set gray_level = ${gray_level_min};
while(${gray_level} <= ${gray_level_max})
@ width_start = ${region_width} * (${gray_level} - ${gray_level_min});
@ width_end = ${width_start} + ${region_width};

#set draw_string = "${draw_string} -fill ${token}rgb(${gray_level},${gray_level},${gray_level})${token} -draw ${token}rectangle ${width_start},0,${width_end},${image_height}${token} ";

magick -size ${region_width}x${region_height} xc:none -fill "rgb(${gray_level},${gray_level},${gray_level})" -draw "rectangle 0,0,${region_width},${region_height}" -compress none ${output_dir}/${stripe_root_name}.ppm

composite -geometry ${region_width}x${region_height}+${width_start}+0 ${output_dir}/${stripe_root_name}.ppm ${output_dir}/${input_root_name}.ppm ${output_dir}/${input_root_name}.ppm;

@ gray_level = ${gray_level} + 1;
end

rm -f ${output_dir}/${stripe_root_name}.ppm;

magick ${output_dir}/${input_root_name}.ppm -compress none ${output_dir}/${input_root_name}.pgm;
rm -f ${output_dir}/${input_root_name}.ppm;

#set exe_string = "magick -size ${image_width}x${image_height} xc:none ${draw_string} -compress none ${output_dir}/foo.ppm";

foreach diffuser_type (floydsteinberg varcoeff zhou knuth knuthzhou)
foreach scanner_type (scanline serpentine random badrandom hilbert)
set output_root_name = ${input_root_name}_${diffuser_type}_${scanner_type};

${exe_dir}/ErrorDiffuse ${output_dir}/${input_root_name}.pgm ${output_dir}/${output_root_name}.pgm ${diffuser_type} ${scanner_type} "null";

magick ${output_dir}/${output_root_name}.pgm ${output_dir}/${output_root_name}.png
end # foreach scanner_type
end # foreach diffuser_type

endif

# color error diffusion for constant float image
if(1) then

set image_size = 128;
set intensity = 0625; # fraction part

set input_root_name = g${intensity};

set palette_root_name = palette;
echo "0 0 0   1 0 0   0 1 0   0 0 1" > ${output_dir}/${palette_root_name}.txt;

set palette_file_name = ${output_dir}/${palette_root_name}.txt;
#set palette_file_name = "null";

foreach diffuser_type (floydsteinberg varcoeff zhou knuth knuthzhou)
foreach scanner_type (scanline serpentine random badrandom hilbert)
set output_root_name = ${input_root_name}_${diffuser_type}_${scanner_type};

${exe_dir}/ErrorDiffuse -${image_size}.${intensity} ${output_dir}/${output_root_name}.ppm ${diffuser_type} ${scanner_type} ${palette_file_name};

magick ${output_dir}/${output_root_name}.ppm ${output_dir}/${output_root_name}.png

end # foreach scanner_type
end # foreach diffuser_type

endif

# void cluster
if(1) then

set grid_size = 128;
set num_classes = 3;
set num_samples = "42 42 42";
set relative_splat_size = 1.0;
set init_swap_size = 8;
set min_num_iterations = 0;

foreach method (discrete gridregular gridjitter whiterandom)
foreach weighting_scheme (fair rainbow)
set output_root_name = voidcluster_${method}_${num_classes}c_${weighting_scheme};

if(${method} == gridjitter) then
set trough_candidate_size = ${init_swap_size};
set trough_jitter_radius = 1;
else if(${method} == whiterandom) then
set trough_candidate_size = ${grid_size};
set trough_jitter_radius = "";
else
set trough_candidate_size = "";
set trough_jitter_radius = "";
endif

${exe_dir}/VoidCluster ${method} ${grid_size} ${num_classes} ${num_samples} ${weighting_scheme} ${relative_splat_size} ${init_swap_size} ${trough_candidate_size} ${trough_jitter_radius} ${min_num_iterations} ${output_dir}/${output_root_name}.ppm
magick ${output_dir}/${output_root_name}.ppm ${output_dir}/${output_root_name}.png

${exe_dir}/VoidClusterVis ${method} ${grid_size} ${num_classes} ${num_samples} ${weighting_scheme} ${relative_splat_size} ${init_swap_size} ${trough_candidate_size} ${trough_jitter_radius} ${min_num_iterations} ${output_dir}/${output_root_name}_vis.ppm
magick ${output_dir}/${output_root_name}_vis.ppm ${output_dir}/${output_root_name}_vis.png
end
end

endif

# constrained error diffusion
if(1) then

set image_size = 256;

set tag_root_name = artificial;

# tag image
magick -size ${image_size}x${image_size} xc:none -fill "rgb(255,255,255)" -draw "rectangle 0,0,${image_size},${image_size}" -fill "rgb(0,255,255)" -draw "rectangle 32,32,96,96" -fill "rgb(255,0,255)" -draw "rectangle 96,96,160,160" -fill "rgb(255,255,0)" -draw "rectangle 160,160,224,224" -compress none ${output_dir}/${tag_root_name}_color.ppm;

magick ${output_dir}/${tag_root_name}_color.ppm -negate -compress none ${output_dir}/${tag_root_name}_spec.ppm

# input image
set input_color = 127;
set color_r = ${input_color};
set color_g = ${input_color};
set color_b = ${input_color};

set input_root_name = r${color_r}g${color_g}b${color_b};

magick -size ${image_size}x${image_size} xc:none -fill "rgb(${color_r},${color_g},${color_b})" -draw "rectangle 0,0,${image_size},${image_size}" -compress none ${output_dir}/${input_root_name}.ppm;
magick ${output_dir}/${input_root_name}.ppm -compress none ${output_dir}/${input_root_name}.ppm;

# output image

set diffuser_type = zhou;
set scanner_type = serpentine;

set palette_root_name = palette;
echo "0 0 0   0 1 1   1 0 1   1 1 0" > ${output_dir}/${palette_root_name}.txt;

set palette_file_name = ${output_dir}/${palette_root_name}.txt;
set palette_file_name = "null";

set output_root_name = ${tag_root_name}_${input_root_name}_${diffuser_type}_${scanner_type};

# constrained error diffusion
${exe_dir}/ConstrainedErrorDiffuse ${output_dir}/${input_root_name}.ppm ${output_dir}/${output_root_name}.ppm ${diffuser_type} ${scanner_type} ${palette_file_name} ${output_dir}/${tag_root_name}_color.ppm ${output_dir}/${tag_root_name}_spec.ppm;
magick ${output_dir}/${tag_root_name}_spec.ppm ${output_dir}/${tag_root_name}_spec.png

endif
