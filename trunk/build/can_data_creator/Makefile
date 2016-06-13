
TARGET := can_data_creator

all: can_data_creator.c
	gcc -o $(TARGET) -Wall can_data_creator.c
	./$(TARGET).exe
	cp can.txt ../

clean:
	rm -f $(TARGET).exe can.txt
