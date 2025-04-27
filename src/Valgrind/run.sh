#!/bin/bash
cd ../../
OUTPUT_DIR="src/Valgrind/logs"
mkdir -p "$OUTPUT_DIR"

OUTPUT_FILE=$(date +%d.%m.%Y_%H:%M:%S)_logs.txt

docker rmi -f valgrind/decimal
docker build -t valgrind/decimal -f src/Valgrind/Dockerfile .

docker run --rm valgrind/decimal > "$OUTPUT_DIR/$OUTPUT_FILE" 2>&1

echo "TOTAL RESULTS:"
grep "ERROR SUMMARY:" "$OUTPUT_DIR/$OUTPUT_FILE" -c
echo "ERRORS:"
grep "ERROR SUMMARY: [^0][0-9]* errors" "$OUTPUT_DIR/$OUTPUT_FILE" -c