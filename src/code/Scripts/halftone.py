def removeComments(inputFileName, outputFileName):

    input = open(inputFileName, "r")
    output = open(outputFileName, "w")

    output.write(input.readline());

    for line in input:
        if not line.lstrip().startswith("#"):
            output.write(line)

    input.close()
    output.close()
