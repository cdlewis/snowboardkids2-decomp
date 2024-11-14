#!/bin/bash
# Extract the Splat-generated linker script and append the custom patch

# Append the custom definition for D_10D40
echo "PROVIDE(D_10D40 = main_BSS_END);" >> snowboardkids2.ld

