import random

fp = open("sort.txt", "w")
for i in range(16):
	rand1 = random.uniform(-16.0, 20.0)
	rand2 = random.uniform(0, 9.0)
	fp.write("{:4f} {:4f}\n".format(rand1, rand2))

fp.close()