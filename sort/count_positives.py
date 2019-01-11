file = open("sort.txt", 'r')
count=0
count_two=0
for line in file.readlines():
	# convert the first number into double
	a = float(line.split(' ')[0])
	if a >0:
		count+=1
	elif a<0:
		count_two+=1

print(count)
print(count_two)

