cd project1_DO_Linux/FileManager/
make
cp libFileManager.so ../Client/libFileManager.so
cd ../TextSearchComponent
make
cp libtextSearch.so ../Client/libtextSearch.so
cd ../Client
make
./client.exe "../FileManager" "*.h" "vector"

