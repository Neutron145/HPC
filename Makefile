debug:
	cmake -S . -B build/debug -DCMAKE_BUILD_TYPE=Debug
	cmake --build build/debug -j$$(nproc)

release:
	cmake -S . -B build/release -DCMAKE_BUILD_TYPE=Release
	cmake --build build/release -j$$(nproc)

run:
	./build/debug/app/HPC

clean:
	rm -rf build/debug build/release
