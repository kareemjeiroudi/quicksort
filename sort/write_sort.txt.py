import random

fp = open("sort.txt", "w")
for i in range(16):
	rand1 = random.uniform(-16.0, 16.0)
	rand2 = random.uniform(0, 9.0)
	fp.write("{:5f} {:5f}\n".format(rand1, rand2))

fp.close()