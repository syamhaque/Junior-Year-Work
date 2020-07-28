CC = g++
CFLAGS =
COPTFLAGS = -O3 -g
LDFLAGS =

default:
	@echo "=================================================="
	@echo "To build your sorting code, use:"
	@echo "  make hashing    # For hashing"
	@echo "  make prim    	 # For prim"
	@echo "  make extra      # For extra"
	@echo "To clean this subdirectory (remove object files"
	@echo "and other junk), use:"
	@echo "  make clean"
	@echo "=================================================="


hashing: hashing.o
	$(CC) $(COPTFLAGS) -o $@ $^

extra: extra.o
	$(CC) $(COPTFLAGS) -o $@ $^

prim: prim.o
	$(CC) $(COPTFLAGS) -o $@ $^

%.o: %.cc
	$(CC) $(CFLAGS) $(COPTFLAGS) -o $@ -c $<

clean:
	rm -f core *.o *~ queue dlist

# eof
