#!/usr/bin/env bash
set -e
echo "Creating ./bin directory..."
mkdir -p ./bin

echo "Running RecompModTool..."
./RecompModTool ./mod.toml ./bin

echo "Zipping output file into ./bin..."
zip -j ./bin/no_company_coins.zip ./bin/no_company_coins.nrm

echo "Complete"