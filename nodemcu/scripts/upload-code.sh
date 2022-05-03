#!/usr/bin/env bash

folder="../src"

echo "Uploading codes from $folder"

if [ -z $1 ]
then
    for f in $(ls $folder)
    do
        echo "Uploading file $f..."
        nodemcu-tool upload "$folder/$f"
    done
else
    nodemcu-tool upload "$folder/$1"
fi
