// https://learn.microsoft.com/en-us/windows/apps/desktop/modernize/apply-windows-themes?source=recommendations

inline bool IsColorLight(Windows::UI::Color& clr)
{
  return (((5 * clr.G) + (2 * clr.R) + clr.B) > (8 * 128));
}