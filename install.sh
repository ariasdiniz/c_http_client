# This script compiles and install the http_client library. 
# Use it only if your Operating System is Linux and you have the gcc compiler installed.

check_error() {
  if [[ $? != 0 ]]; then
    echo $1
    exit 1
  fi
}

directory="."
all_files=""

readarray -t files < <(find "$directory" -type f \( -name "*.c" \))

for file in "${files[@]}"; do
  gcc -c -fpic $file -o $file.o -I.
  check_error "Error compiling position independent code of file $file."
  all_files+="$file.o "
done

gcc -shared -o libhttp_client.so $all_files
check_error "Error compiling shared object."

for file in "${files[@]}"; do
  rm -r $file.o
done

echo "Compilation of libhttp_client.so finished."

sudo cp libhttp_client.so /usr/lib

check_error "Error copying shared object to /usr/lib."

sudo cp include/httpclient.h /usr/include

check_error "Error copying header file to /usr/include."

rm -r libhttp_client.so

echo "Installation of libhttp_client.so finished."
