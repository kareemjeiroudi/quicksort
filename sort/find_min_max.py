file = open("sort.txt", 'r')
min=0
max=0
for line in file.readlines():
	# convert the first number into double
	a = float(line.split(' ')[0])
	if a >max:
		max=a
	elif a<min:
		min=a

print(max)
print(min)