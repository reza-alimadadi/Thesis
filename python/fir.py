import random
import sys

maxNode = int(sys.argv[1])

specFile = open("../input/spec", "w")

for i in range(0, maxNode):
	specFile.write("%t" + str(i) + " = " + "m" + str(i) + ".*." + str(random.randint(0, 10)) + 
					" " + "%i" + str(2 * i) + ", " + "%i" + str(2 * i + 1) + "\n")

for i in range(0, maxNode - 1):
	if (maxNode - i == 2):
		specFile.write("%o" + str(0))
	else:
		specFile.write("%k" + str(i))

	specFile.write(" = " + "a" + str(i) + ".+." + str(random.randint(0, 10)) + " ")

	if (i == 0):
		specFile.write("%t" + str(i))
	else:
		specFile.write("%k" + str(i - 1))

	specFile.write(", " + "%t" + str(i + 1) + "\n")

specFile.close()


inputFile = open("../input/inputs", "w")

for i in range(0, maxNode):
	inputFile.write("%i" + str(2 * i) + "\n")
	inputFile.write("%i" + str(2 * i + 1) + " " + str(random.randint(1, 32768)) + "\n")

inputFile.close()


outputFile = open("../input/outputs", "w")
outputFile.write("%o" + str(0) + "\n")
outputFile.close()
