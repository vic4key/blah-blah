# https://github.com/tmquan/classification/blob/main/dcm2png.ipynb

def dcm_to_png(dcm_file, png_file=None, fix_monochrome=True, apply_windowing=True, long_edge=512):
    """ Function to convert from a DICOM image to png
        @param dcm_file: An opened file like object to read te dicom data
        @param png_file: An opened file like object to write the png data
    """

    # Extracting data from the mri file
    dicom = pydicom.dcmread(dcm_file)
    image = dicom.pixel_array
    # shape = image.shape

    
    # if apply_windowing:
    # image = pydicom.pixel_data_handlers.util.apply_modality_lut(image, dicom)
    image = pydicom.pixel_data_handlers.util.apply_voi_lut(image, dicom)
    # image = pydicom.pixel_data_handlers.util.apply_windowing(image, dicom)

    # Scale to 0~1
    image -= ( np.min(image) )
    image /= ( np.max(image) + 1e-6 )

    # Invert if not mono1
    if fix_monochrome and dicom.PhotometricInterpretation != "MONOCHROME1":
        image = 1.0 - image

    if long_edge is not None:
        # Resize long edge
        factor = float(long_edge) / max(image.shape[:2])
        shape = (int(image.shape[0] * factor), int(image.shape[1] * factor))
        image = skimage.transform.resize(image, shape)

    image = (image * 255).astype(np.uint8)
    
    if png_file is None:
        png_file = dcm_file.replace("dcm", "png")
    skimage.io.imsave(png_file, image)