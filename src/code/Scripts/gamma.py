import sys
import os
import argparse
import glob

# works with spatial_gamma_correction
# https://www.quora.com/In-what-ways-can-I-do-dithering-in-Photoshop

def main():

    bin_dir = os.path.join(".", "bin", "Release");
    output_dir = ".";
    size = 64;
    #diffuser = "floydsteinberg";
    #diffuser = "varcoeff";
    diffuser = "zhou";
    #scanner = "scanline"; # diagonal direction bias
    scanner = "serpentine";

    grays = [0, 64, 128, 192, 255];

    if True:
        palette_file_name = "null";
    else:
        palette_file_name = os.path.join(output_dir, "palette.txt");
        palette = open(palette_file_name, "w");
        palette.write("0 0 0  1 1 1"); # black and white
        palette.close();

    for gray in grays:
        input_base_name = "gray_" + str(gray);
        input_pgm = os.path.join(output_dir, input_base_name + ".pgm");

        output_base_name = diffuser + "_" + scanner + "_" + str(gray);
        output_pgm = os.path.join(output_dir, output_base_name + ".pgm");
        output_png = os.path.join(output_dir, output_base_name + ".png");

        if True:
            command = "magick -size " + str(size)+"x"+str(size) + " xc:" + "gray(" + str(gray) + ")" + " -depth 8 -compress none " + input_pgm;
            os.system(command);
            input_spec = input_pgm;
        else:
            input_spec = str(size) + "." + str(gray)

        command = os.path.join(bin_dir, "ErrorDiffuse") + " " + input_spec + " " + output_pgm + " " + diffuser + " " + scanner + " " + palette_file_name;
        
        #print(command);
        os.system(command);

        command = "magick " + output_pgm + " " + output_png;
        os.system(command);
        os.remove(output_pgm);

        # check
        command = "magick " + output_png  + " -resize 1x1 txt:-";
        os.system(command);

    # cleanup
    if os.path.exists(palette_file_name):
        os.remove(palette_file_name);

if __name__ == "__main__":
    main();
