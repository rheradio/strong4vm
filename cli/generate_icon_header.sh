#!/bin/bash
# Generate icon header file from figures/icon.txt
# Usage: ./generate_icon_header.sh

ICON_FILE="../figures/icon.txt"
OUTPUT_FILE="icon_embedded.hh"

if [ ! -f "$ICON_FILE" ]; then
    echo "Error: Icon file not found: $ICON_FILE"
    exit 1
fi

# Start writing the header file
cat > "$OUTPUT_FILE" << 'EOF'
// Auto-generated file - DO NOT EDIT
// Generated from figures/icon.txt

#ifndef ICON_EMBEDDED_HH
#define ICON_EMBEDDED_HH

#include <string>

namespace strong4vm {

const std::string ICON_ASCII = R"(
EOF

# Append the icon content
cat "$ICON_FILE" >> "$OUTPUT_FILE"

# Close the raw string literal and namespace
cat >> "$OUTPUT_FILE" << 'EOF'
)";

} // namespace strong4vm

#endif // ICON_EMBEDDED_HH
EOF

echo "Generated $OUTPUT_FILE successfully"
