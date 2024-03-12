# This script compiles and install the http_client library. 
# Use it only if your Operating System is Linux and you have the gcc compiler installed.

directory="."
all_files=""

readarray -t files < <(find "$directory" -type f \( -name "*.c" \))

for file in "${files[@]}"; do
  gcc -c -fpic $file -o $file.o -I.
  if [[ $? != 0  ]]; then
    echo "Error compiling position independent code."
    exit 1
  fi
  all_files+="$file.o "
done

gcc -shared -o libhttp_client.so $all_files
if [[ $? != 0 ]]; then
  echo "Error compiling shared object."
  exit 1
fi

for file in "${files[@]}"; do
  rm -r $file.o
done

echo "Compilation of libhttp_client.so finished."

sudo cp libhttp_client.so /usr/lib

if [[ $? != 0 ]]; then
  echo "Error copying shared object to /usr/lib."
  exit 1
fi

sudo cp include/httpclient.h /usr/include

if [[ $? != 0 ]]; then
  echo "Error copying header file to /usr/include."
  exit 1
fi

rm -r libhttp_client.so

echo "Installation of libhttp_client.so finished."
