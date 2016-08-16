import re

calculate = []
estimate = []

with open("estLog") as estFile:
	for line in estFile:
		estimate.append( re.findall('(%\w\d+)\: ([\d.]+)', line))

with open("calcLog") as calcFile:
	for line in calcFile:
		calculate.append( re.findall ('(%\w\d+)\: ([\d.]+)', line))

for caclResult in calculate:
	for estResult in estimate:
		if (caclResult[0][0] == estResult[0][0]):
			print("Estimated Var" + " " + estResult[0][0] + ": " + estResult[0][1])
			print("Calculated Var" + " " + caclResult[0][0] + ": " + caclResult[0][1])
			relative = (float(caclResult[0][1]) - float(estResult[0][1])) / float(caclResult[0][1]) * 100
			print("Relative" + " " + caclResult[0][0] + ": " + str(relative))
			print("********************************************************************")

