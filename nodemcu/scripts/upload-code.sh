#!/usr/bin/env bash

folder="../src"

echo "Uploading codes from $folder"

for f in $(ls $folder)
do
    echo "Uploading file $f..."
    nodemcu-tool upload "$folder/$f"
done
