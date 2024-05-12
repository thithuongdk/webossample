#!/bin/bash

# Đường dẫn đến thư mục chứa các tệp SVG
input_dir="./icons/svg"

# Đường dẫn đến thư mục nơi bạn muốn lưu các tệp PNG
output_dir="./icons/png"

# Kiểm tra xem cả hai thư mục có tồn tại không
if [ -d "$input_dir" ] && [ -d "$output_dir" ]; then
    # Duyệt qua tất cả các tệp SVG trong thư mục và chuyển đổi chúng sang PNG
    for file in "$input_dir"/*.svg; do
        # Kiểm tra xem tệp có phải là một tệp SVG không
        if [ -f "$file" ]; then
            # Tạo tên tệp PNG đầu ra bằng cắt bỏ phần mở rộng và thêm .png
            output_file="$output_dir/$(basename "${file%.*}.png")"
            # Chuyển đổi tệp SVG sang PNG mà không có nền bằng Inkscape
            # inkscape -z -e "$output_file" -h 800 -w 800 "$file"
            convert "$file" -background none -flatten "$output_file"
            # convert "$file" -alpha transparent "$output_file"
            # convert "$file" -transparent white "$output_file"
            echo "done $output_file"
        fi
    done
else
    echo "Một trong hai thư mục không tồn tại."
fi