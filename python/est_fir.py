import re

multError = [0, 0, 0, 0.480713, 5.36938, 37.7258, 214.469, 1072.01, 4926.78, 21379.8, 89295.7]
addError = [0, 0.25, 1.25, 5.25, 21.25, 85.25, 341.25, 1365.25, 5461.25, 21845.2, 87381.2]

# this is only for fir spec
systemError = 0.0;
with open("../input/spec") as specFile:
	for line in specFile:
		errorProperty = re.findall('\w+\.([*+])\.(\d+)', line)
		if (errorProperty[0][0] == "*"):
			systemError += multError[int(errorProperty[0][1])]

		elif (errorProperty[0][0] == '+'):
			systemError += addError[int(errorProperty[0][1])]

print("Estimated Var %o0: " + str(systemError))