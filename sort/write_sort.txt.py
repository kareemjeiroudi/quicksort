import random

fp = open("sort.txt", "w")
for i in range(14000):
	rand1 = random.uniform(-200, 40001.0)
	rand1 = round(rand1, 3)
	rand2 = random.uniform(-1.0, 5.0)
	rand2 = round(rand2, 3)
	fp.write("{:0f} {:0f}\n".format(rand1, rand2))

fp.close()