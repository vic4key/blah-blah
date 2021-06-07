// import/export vtkPolyData as .vtk

// https://stackoverflow.com/questions/29796962/how-to-save-a-vtksphere-to-a-vtk-file
const auto fn_export_vtk_poly_data = [](vtkSmartPointer<vtkPolyData> data, const std::string& file_name) -> void
{
  vtkNew<vtkPolyDataWriter> writer;
  writer->SetFileName(file_name.c_str());
  writer->SetInputData(data);
  writer->Write();
};

// https://kitware.github.io/vtk-examples/site/Cxx/Points/PoissonExtractSurface/
const auto fn_import_vtk_poly_data = [](vtkSmartPointer<vtkPolyData>* data, const std::string& file_name) -> void
{
  vtkNew<vtkPolyDataReader> reader;
  reader->SetFileName(file_name.c_str());
  reader->Update();
  *data = reader->GetOutput();
};

vtkSmartPointer<vtkPolyData> data;
fn_export_vtk_poly_data(data, "test.vtk");
fn_export_vtk_poly_data(&data, "test.vtk");