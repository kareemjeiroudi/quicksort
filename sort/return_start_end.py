def return_start_end(b_min):
  	threads = [0, 1, 2, 3, 4, 5, 6, 7]

  	num_threads = 8
  	start_ends = [[0 for x in range(2)] for y in range(num_threads)]
  	for me in threads:
  		# start = me * (b_min/num_threads -1) + me;
  		start = me *(b_min/num_threads +1) - me 
	  	# end = (me+1) * (b_min/num_threads -1) + me;
	  	end = (me+1) *(b_min/num_threads +1) - (me+1)
	  	start_ends[me][0] = start
	  	start_ends[me][1] = end
  	return start_ends

def main():
	print(return_start_end(101))


if __name__ == '__main__':
	main()