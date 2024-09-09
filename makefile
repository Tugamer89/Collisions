compile:
	g++ ./source/*.cpp -lsfml-graphics -lsfml-window -lsfml-system -Wno-narrowing -O3 -o collisions

run:
	./collisions

run-reg:
	rm -f output.mp4
	./collisions reg
	ffmpeg -framerate 60 -i screenshots/frame_%d.png -c:v libx264 output.mp4
	rm -f screenshots/*
