import os
import shutil
import PyVutils as vu

# configurations

def fn_default(tag): return str(tag.value)

# list tags to print
list_tags_to_use = {
    # "Study Instance UID": (0x0020,0x000D),
    # "Series Instance UID": (0x0020,0x000E),
    # "SOP Instance UID": (0x0008,0x0018),
    # "Referenced Image Sequence - Referenced SOP Instance UID": [(0x0008,0x1140), (0x0008,0x1155)],
    # "Source Image Sequence - Referenced SOP Instance UID": [(0x0008,0x2112), (0x0008,0x1155)],
    "Slice Location": (0x0020,0x1041),
    "Image Position": (0x0020, 0x0032, lambda tag: "%.3f" % tag.value[1]),
    # "Series Time": (0x0008,0x0031),
    # "Content Date": (0x0008,0x0023),
    # "Content Time": (0x0008,0x0033),
    # "Acquisition DateTime": (0x0008,0x002A),
    # "Image Orientation": (0x0020, 0x0037),
}

# filter series to print list by its number of images
list_image_count_in_usable_series = None
# list_image_count_in_usable_series = [159, 197] # Case2
# list_image_count_in_usable_series = [87] # Verona
# list_image_count_in_usable_series = [69, 123, 128] # Xijing Site

# number of images to print
number_of_images_to_use = None
# number_of_images_to_use = 5
# number_of_images_to_use = max(list_image_count_in_usable_series)

file_dir = R"path\to\folder\contains\list\image\series"
# file_dir = R"path\to\folder\dicom-4d-series\Data-4DCT\Verona"
# file_dir = R"path\to\folder\dicom-4d-series\DICOM\Xijing Site"
# file_dir = R"path\to\folder\dicom-4d-series\DICOM\Xijing Site"

report_name = None
# report_name = "20190419_4DCT Data_4DCT"
# report_name = "Verona"
# report_name = "Xijing Site"
if report_name is None: report_name = os.path.basename(file_dir)



# load dicom files and sort slices in each series by slice location then filter usable series

def fn_sort(ds):
    if [0x0020, 0x1041] in ds:   # Slice Location
        return ds[0x0020, 0x1041].value
    elif [0x0020, 0x0032] in ds: # Image Position
        return float(ds[0x0020, 0x0032][2])
    raise RuntimeError(f"missing field for sorting by z-position '{ds.filename}'")

usable_series = []
usable_series_image_counts = set()
list_series = vu.load_dicom_directory(file_dir, image_only=True)
for series in list_series.values():
    if not list_image_count_in_usable_series or len(series) in list_image_count_in_usable_series:
        series.sort(key=fn_sort)
        usable_series.append(series)
        usable_series_image_counts.add(len(series))
usable_series.sort(key = lambda series: len(series))

if len(usable_series) == 0:
    raise "no image series"

if number_of_images_to_use is None:
    if len(usable_series_image_counts) > 0:
        number_of_images_to_use = min(usable_series_image_counts)
    else:
        raise "missing `number_of_images_to_use`"
else:
    if number_of_images_to_use > min(usable_series_image_counts):
        raise "invalid `number_of_images_to_use`"

# xls - header row of series name

lines = "\t"
for series in usable_series:
    file_name = os.path.dirname(series[0].filename).split("\\")[-1]
    file_name = "%s (%d images)" % (file_name, len(series))
    # file_name = "%s (%d images)" % (series[0][0x0008, 0x103E].value, len(series)) # series description
    file_name = vu.clean_file_name(file_name)
    lines += "%s\t" % file_name
lines += "\n"

# xls - list rows of slice locations

for i in range(0, number_of_images_to_use):
    for j, tag in enumerate(list_tags_to_use.values()):
        tag_name = list(list_tags_to_use.keys())[j]
        fn = fn_default
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
            # print(f"=> '{file_path}'")

            try:
                if type(tag) is list: # sequence
                    _1, _2 = tag[0]
                    group, element = tag[1]
                    line += fn(series[i][_1,_2][0][group, element])
                else:
                    if len(tag) == 3: group, element, fn = tag
                    else: group, element = tag
                    line += fn(series[i][group, element])
            except: pass # print(f"No tag '{str(tag)} - {tag_name}' in '{series[i].filename}'")
            line += "\t"
        lines += f"{line}\n"

# xls - write to file

report_file_path = os.path.join(file_dir, f"report_{report_name}.xls")
vu.write_file(report_file_path, lines.encode("utf-8"))

print("Done!!")