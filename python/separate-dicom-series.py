import os, shutil
from PyVutils import DCM, FS

def main():
    dicom_dir = R"path\to\folder\that\contains\series"
    separate_dir = R".\separated_series"

    list_series = DCM.load_dicom_directory(dicom_dir)
    if len(list_series) == 0:
        print("No any series")
        return

    for series_name, series in list_series.items():

        if len(series) == 0: continue
        ds = series[0]

        # Patient Name _ Patient ID _ Study ID _ Study Instance UID
        study_folder = f"{ds[0x0010, 0x0010].value}_{ds[0x0010, 0x0020].value}_{ds[0x0020, 0x0010].value}_{ds[0x0020, 0x0000D].value}"
        study_folder = FS.clean_file_name(study_folder)
        study_dir = os.path.join(separate_dir, study_folder)
        if not os.path.exists(study_dir): os.makedirs(study_dir)

        # Modality _ Series Instance UID
        series_folder = f"{ds[0x0008, 0x0060].value}_{series_name}"
        series_folder = FS.clean_file_name(series_folder)
        series_dir = os.path.join(study_dir, series_folder)
        if not os.path.exists(series_dir): os.makedirs(series_dir)

        for ds in series:
            src_file_path = ds.filename
            dst_file_path = os.path.join(series_dir, ds.filename.split('\\')[-1])
            shutil.copy(src_file_path, dst_file_path)

    print("Done!!")

if __name__ == "__main__": main()
