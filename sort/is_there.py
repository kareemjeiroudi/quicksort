# 0.000000 and c_end 32.492047 
file = open("sort.txt", 'r')
count=0
for line in file.readlines():
	# convert the first number into double
	a = float(line.split(' ')[1])
	if a > 0.0 and a < 32.492047:
		print("There is {}".format(a))
		count+=1

print(count)