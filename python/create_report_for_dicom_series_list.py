import os
import shutil
from PyVutils import DCM, File

# configurations

# list tags to print
list_tags_to_use = {
    "Study Instance UID": (0x0020,0x000D),
    "Series Instance UID": (0x0020,0x000E),
    "SOP Instance UID": (0x0008,0x0018),
    "Referenced Image Sequence - Referenced SOP Instance UID": [(0x0008,0x1140), (0x0008,0x1155)],
    "Source Image Sequence - Referenced SOP Instance UID": [(0x0008,0x2112), (0x0008,0x1155)],
    "Slice Location": (0x0020,0x1041),
    "Series Time": (0x0008,0x0031),
    "Content Date": (0x0008,0x0023),
    "Content Time": (0x0008,0x0033),
    "Acquisition DateTime": (0x0008,0x002A),
}

# filter series to print list by its number of images
list_image_count_in_usable_series = [159, 197] # Case2
# list_image_count_in_usable_series = [87] # Verona
# list_image_count_in_usable_series = [69, 123, 128] # Xijing Site

# number of images to print
number_of_images_to_use = 5
# number_of_images_to_use = max(list_image_count_in_usable_series)

file_dir = R"path\to\folder\dicom-4d-series\Data-4DCT\4DCT2\Case2"
# file_dir = R"path\to\folder\dicom-4d-series\Data-4DCT\Verona"
# file_dir = R"path\to\folder\dicom-4d-series\DICOM\Xijing Site"

report_name = "Case2"
# report_name = "20190419_4DCT Data_4DCT"
# report_name = "Verona"
# report_name = "Xijing Site"



# load dicom files and sort slices in each series by slice location then filter usable series

usable_series = []
list_series = DCM.Loadirectory(file_dir)
for series in list_series.values():
    if len(series) in list_image_count_in_usable_series:
        series.sort(key = lambda ds: ds[0x0020, 0x1041].value) # slice location
        usable_series.append(series)
usable_series.sort(key = lambda series: len(series))

# xls - header row of series name

lines = "\t"
for series in usable_series:
    file_name = os.path.dirname(series[0].filename).split("\\")[-1]
    file_name = "%s (%d images)" % (file_name, len(series))
    # file_name = "%s (%d images)" % (series[0][0x0008, 0x103E].value, len(series)) # series description
    file_name = File.CleanFileName(file_name)
    lines += "%s\t" % file_name
lines += "\n"

# xls - list rows of slice locations

for i in range(0, number_of_images_to_use):
    for j, tag in enumerate(list_tags_to_use.values()):
        tag_name = list(list_tags_to_use.keys())[j]
        line  = f"\nIMAGE #{i + 1}\n" if j == 0 else ""
        line += f"{tag_name}\t"
        for series in usable_series:

            # # copy to create a lite series (based on slice location) from a original series
            # file_parts = series[i].filename.split('\\')
            # file_dir  = RF"path\to\folder\dicom-4d-series\Data-4DCT\4DCT2\Case2_Lite\{file_parts[-2]}"
            # file_name = file_parts[-1]
            # file_path = RF"{file_dir}\{file_name}"
            # if not os.path.exists(file_dir): os.makedirs(file_dir)
            # src_file_path = series[i].filename
            # dst_file_path = file_path
            # shutil.copy(src_file_path, dst_file_path)

            print(f"=> '{file_path}'")
            try:
                if type(tag) is list: # sequence
                    _1, _2 = tag[0]
                    group, element = tag[1]
                    line += str(series[i][_1,_2][0][group, element].value)
                else:
                    group, element = tag
                    line += str(series[i][group, element].value)
            except: pass # print(f"No tag '{str(tag)} - {tag_name}' in '{series[i].filename}'")
            line += "\t"
        lines += f"{line}\n"

# xls - write to file

File.Write(RF"report_{report_name}.xls", lines.encode("utf-8"))
print("Done!!")