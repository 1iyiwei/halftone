#!/bin/tcsh -f

set output_dir = data;

if(0) then

foreach gray_level (0 64 127 128 192 255)

set image_size = 64;

set input_root_name = gray${gray_level};
set diffuser_type = floydsteinberg;
set scanner_type = scanline;
set output_root_name = ${input_root_name}_${diffuser_type}_${scanner_type};

convert -size ${image_size}x${image_size} xc:none -fill "rgb(${gray_level},${gray_level},${gray_level})" -draw "rectangle 0,0,${image_size},${image_size}" -compress none ${output_dir}/${input_root_name}.ppm;
convert ${output_dir}/${input_root_name}.ppm -compress none ${output_dir}/${input_root_name}.pgm;
rm -f ${output_dir}/${input_root_name}.ppm;

./src/Main/Release/ErrorDiffuse ${output_dir}/${input_root_name}.pgm ${output_dir}/${output_root_name}.pgm ${diffuser_type} ${scanner_type} "null";

end #foreach gray_level
endif

if(0) then

set input_root_name = ramp;
set region_height = 64;
set region_width = 2;
set gray_level_min = 0;
set gray_level_max = 255;

set image_height = ${region_height};
@ image_width = 1 + ${gray_level_max} - ${gray_level_min};
@ image_width = ${image_width} * ${region_width};

set stripe_root_name = ${input_root_name}_stripe;

convert -size ${image_width}x${image_height} xc:none -fill "rgb(0,0,0)" -draw "rectangle 0,0,${image_width},${image_height}" -compress none ${output_dir}/${input_root_name}.ppm

set draw_string = "";

set token = \";

set gray_level = ${gray_level_min};
while(${gray_level} <= ${gray_level_max})
@ width_start = ${region_width} * (${gray_level} - ${gray_level_min});
@ width_end = ${width_start} + ${region_width};

#set draw_string = "${draw_string} -fill ${token}rgb(${gray_level},${gray_level},${gray_level})${token} -draw ${token}rectangle ${width_start},0,${width_end},${image_height}${token} ";

convert -size ${region_width}x${region_height} xc:none -fill "rgb(${gray_level},${gray_level},${gray_level})" -draw "rectangle 0,0,${region_width},${region_height}" -compress none ${output_dir}/${stripe_root_name}.ppm

composite -geometry ${region_width}x${region_height}+${width_start}+0 ${output_dir}/${stripe_root_name}.ppm ${output_dir}/${input_root_name}.ppm ${output_dir}/${input_root_name}.ppm;

@ gray_level = ${gray_level} + 1;
end

rm -f ${output_dir}/${stripe_root_name}.ppm;

convert ${output_dir}/${input_root_name}.ppm -compress none ${output_dir}/${input_root_name}.pgm;
rm -f ${output_dir}/${input_root_name}.ppm;

#set exe_string = "convert -size ${image_width}x${image_height} xc:none ${draw_string} -compress none ${output_dir}/foo.ppm";

set diffuser_type = floydsteinberg;

foreach scanner_type (scanline serpentine random)
set output_root_name = ${input_root_name}_${diffuser_type}_${scanner_type};

./src/Main/Release/ErrorDiffuse ${output_dir}/${input_root_name}.pgm ${output_dir}/${output_root_name}.pgm ${diffuser_type} ${scanner_type} "null";
end # foreach scanner_type

endif

# input from Ostromoukhov 2001
if(0) then

set input_root_name = butterfly;

convert ${output_dir}/${input_root_name}.pgm -transpose -compress none -resize 512x512 ${output_dir}/${input_root_name}_transpose.pgm

set palette_root_name = palette;

echo "0 0.25 0.5 0.75 1" > ${output_dir}/${palette_root_name}.txt;

set palette_file_name = ${output_dir}/${palette_root_name}.txt;
set palette_file_name = "null";

foreach diffuser_type (floydsteinberg varcoeff zhou knuth knuthzhou)
foreach scanner_type (scanline serpentine random badrandom hilbert)
set output_root_name = ${input_root_name}_${diffuser_type}_${scanner_type};

./src/Main/Release/ErrorDiffuse ${output_dir}/${input_root_name}_transpose.pgm ${output_dir}/${output_root_name}.pgm ${diffuser_type} ${scanner_type} ${palette_file_name};

convert ${output_dir}/${output_root_name}.pgm -transpose -compress none ${output_dir}/${output_root_name}.pgm

end # foreach scanner_type
end # foreach diffuser_type

endif

# input from Lenna
if(0) then

set input_root_name = Lenna;

convert ${output_dir}/${input_root_name}.ppm -transpose -compress none ${output_dir}/${input_root_name}_transpose.ppm

set palette_file_name = "null";

foreach diffuser_type (floydsteinberg varcoeff zhou knuth knuthzhou)
foreach scanner_type (scanline serpentine random badrandom hilbert)
set output_root_name = ${input_root_name}_${diffuser_type}_${scanner_type};

./src/Main/Release/ErrorDiffuse ${output_dir}/${input_root_name}_transpose.ppm ${output_dir}/${output_root_name}.ppm ${diffuser_type} ${scanner_type} ${palette_file_name};

convert ${output_dir}/${output_root_name}.ppm -transpose -compress none ${output_dir}/${output_root_name}.ppm

end # foreach scanner_type
end # foreach diffuser_type

endif

# color error diffusion for constant image, i.e. uniform sampling
if(0) then

set image_size = 128;

set color_r = 16;
set color_g = 16;
set color_b = 16;

set input_root_name = r${color_r}g${color_g}b${color_b};
set diffuser_type = floydsteinberg;
set diffuser_type = varcoeff;
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

# discrete void cluster
if(0) then

set grid_size = 128;
set num_classes = 3;
set num_samples = "42 42 42";
set relative_splat_size = 1.0;
set init_swap_size = 8;
set min_num_iterations = 0;

foreach method (discrete gridregular gridjitter whiterandom)
foreach weighting_scheme (fair) #rainbow

set output_root_name = voidcluster_${method}_${num_classes}c_${weighting_scheme};

if(${method} == gridjitter) then
set trough_candidate_size = 5;
set trough_jitter_radius = 1;
else if(${method} == whiterandom) then
set trough_candidate_size = ${grid_size};
set trough_jitter_radius = "";
else
set trough_candidate_size = "";
set trough_jitter_radius = "";
endif

./src/Main/Release/VoidCluster ${method} ${grid_size} ${num_classes} ${num_samples} ${weighting_scheme} ${relative_splat_size} ${init_swap_size} ${trough_candidate_size} ${trough_jitter_radius} ${min_num_iterations} ${output_dir}/${output_root_name}.ppm

./src/Main/Release/VoidClusterVis ${method} ${grid_size} ${num_classes} ${num_samples} ${weighting_scheme} ${relative_splat_size} ${init_swap_size} ${trough_candidate_size} ${trough_jitter_radius} ${min_num_iterations} ${output_dir}/${output_root_name}_vis.ppm

end
end

endif

# microsoft tag, gray input
if(0) then

set tag_root_name = maosphere;

set image_size = 346;

set tag_region = "${image_size}x${image_size}+380+97";

set input_color = 127;
set color_r = ${input_color};
set color_g = ${input_color};
set color_b = ${input_color};

set input_root_name = r${color_r}g${color_g}b${color_b};

set diffuser_type = zhou;
set scanner_type = serpentine;

set palette_root_name = palette;
echo "0 0 0   0 1 1   1 0 1   1 1 0" > ${output_dir}/${palette_root_name}.txt;

set palette_file_name = ${output_dir}/${palette_root_name}.txt;
#set palette_file_name = "null";

set output_root_name = ${input_root_name}_${diffuser_type}_${scanner_type};

# tag image
convert ${output_dir}/${tag_root_name}.wmf -crop ${tag_region} -compress none ${output_dir}/${tag_root_name}_color.ppm

convert ${output_dir}/${tag_root_name}_color.ppm -negate -compress none ${output_dir}/${tag_root_name}_spec.ppm

# input image
convert -size ${image_size}x${image_size} xc:none -fill "rgb(${color_r},${color_g},${color_b})" -draw "rectangle 0,0,${image_size},${image_size}" -compress none ${output_dir}/${input_root_name}.ppm;
convert ${output_dir}/${input_root_name}.ppm -compress none ${output_dir}/${input_root_name}.ppm;

# constrained error diffusion
./src/Main/Release/ConstrainedErrorDiffuse ${output_dir}/${input_root_name}.ppm ${output_dir}/${output_root_name}.ppm ${diffuser_type} ${scanner_type} ${palette_file_name} ${output_dir}/${tag_root_name}_color.ppm ${output_dir}/${tag_root_name}_spec.ppm;

endif

# microsoft tag, image input
if(0) then

set tag_root_name = maosphere;

set image_size = 346;

set tag_region = "${image_size}x${image_size}+380+97";

set input_root_name = Lenna;

set diffuser_type = zhou;
set scanner_type = serpentine;

set palette_root_name = palette;
echo "0 0 0   0 1 1   1 0 1   1 1 0" > ${output_dir}/${palette_root_name}.txt;

set palette_file_name = ${output_dir}/${palette_root_name}.txt;
set palette_file_name = "null";

set output_root_name = ${tag_root_name}_${input_root_name}_${diffuser_type}_${scanner_type};

# tag image
convert ${output_dir}/${tag_root_name}.wmf -crop ${tag_region} -compress none ${output_dir}/${tag_root_name}_color.ppm

# debug
# convert -size ${image_size}x${image_size} xc:none -fill "rgb(255,255,255)" -draw "rectangle 0,0,${image_size},${image_size}" -compress none ${output_dir}/${tag_root_name}_color.ppm;

convert ${output_dir}/${tag_root_name}_color.ppm -negate -compress none ${output_dir}/${tag_root_name}_spec.ppm

# input image
convert ${output_dir}/${input_root_name}.png -resize ${image_size}x${image_size} -compress none ${output_dir}/${input_root_name}_comment.ppm;
set comment_char = \#;
grep -v \\${comment_char} ${output_dir}/${input_root_name}_comment.ppm > ${output_dir}/${input_root_name}.ppm

# constrained error diffusion
./src/Main/Release/ErrorDiffuse ${output_dir}/${input_root_name}.ppm ${output_dir}/${output_root_name}_unconstrained.ppm ${diffuser_type} ${scanner_type} ${palette_file_name};

./src/Main/Release/ConstrainedErrorDiffuse ${output_dir}/${input_root_name}.ppm ${output_dir}/${output_root_name}.ppm ${diffuser_type} ${scanner_type} ${palette_file_name} ${output_dir}/${tag_root_name}_color.ppm ${output_dir}/${tag_root_name}_spec.ppm;

endif

# convert my photo for HKU
if(0) then

set input_root_name = wei;

convert ${output_dir}/${input_root_name}.jpg -compress none -resize 700% ${output_dir}/${input_root_name}.ppm

set diffuser_type = zhou;
set scanner_type = serpentine;
set output_root_name = ${input_root_name}_${diffuser_type}_${scanner_type};

./src/Main/Release/ErrorDiffuse ${output_dir}/${input_root_name}.ppm ${output_dir}/${output_root_name}.ppm ${diffuser_type} ${scanner_type} "null";

convert ${output_dir}/${output_root_name}.ppm ${output_dir}/${output_root_name}.png
endif
