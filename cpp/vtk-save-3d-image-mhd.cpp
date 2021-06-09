// @refer vtk-conv-poly-data-to-image-data.cpp

static void vtk_save_poly_data_to_mhd(const vtkSmartPointer<vtkPolyData>& pd, const std::string& file_path)
{
  auto data = vtk_conv_poly_data_to_image_data(pd);

  vtkNew<vtkMetaImageWriter> writer;
  writer->SetFileName(file_path.c_str());
  writer->SetInputData(data);
  writer->Write();
}
