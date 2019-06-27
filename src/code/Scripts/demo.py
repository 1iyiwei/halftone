import sys
import os
import argparse
import glob
from halftone import *

'''
Better quality than Photoshop
https://www.quora.com/In-what-ways-can-I-do-dithering-in-Photoshop
'''

def findBaseName(input_file_path):
    path, file = os.path.split(input_file_path);
    parts = file.split(".");
    return parts[0];

def main():

    bin_dir = os.path.join(".", "bin", "Release");
    output_dir = ".";
    #diffuser = "floydsteinberg";
    #diffuser = "varcoeff";
    diffuser = "zhou";
    scanner = "scanline";

    parser = argparse.ArgumentParser(description='halftone demo');

    parser.add_argument("input_file_name", nargs=1, help = "input_file_name");

    args = parser.parse_args()

    input_file_names = args.input_file_name;

    if True:
        palette_file_name = "null";
    else:
        palette_file_name = os.path.join(output_dir, "palette.txt");
        palette = open(palette_file_name, "w");
        palette.write("0 0 0  1 1 1"); # black and white
        palette.close();

    for input_file_name in input_file_names:
        input_base_name = findBaseName(input_file_name);
        input_pgm = os.path.join(output_dir, input_base_name + ".pgm");
        input_clean_pgm = os.path.join(output_dir, input_base_name + "_clean.pgm");

        output_base_name = input_base_name + "_" + diffuser + "_" + scanner;
        output_pgm = os.path.join(output_dir, output_base_name + ".pgm");
        output_png = os.path.join(output_dir, output_base_name + ".png");

        command = "magick " + input_file_name + " -depth 8 -compress none " + input_pgm;
        os.system(command);

        removeComments(input_pgm, input_clean_pgm);
        
        input_spec = input_clean_pgm;
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
