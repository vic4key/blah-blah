CFloatingDialog* CFloatingDialogManager::GetpFocusingDialog()
{
  // NOTE: If you're debugging and tracing over this line
  // it will return the currently working window like VS IDE window, not floating or the others.
  // https://docs.microsoft.com/en-us/cpp/mfc/reference/cwnd-class?view=msvc-160#getforegroundwindow
  HWND hFocusingWindow = AfxGetApp()->GetMainWnd()->GetForegroundWindow()->GetSafeHwnd();

  if (auto pWnd = CWnd::FromHandle(hFocusingWindow))
  {
    if (auto pImageView = dynamic_cast<CImageView*>(pWnd))
    {
      if (auto pParent = pImageView->GetParent())
      {
        hFocusingWindow = pParent->GetSafeHwnd();
      }
    }
  }

  for (auto& pFloatingDlg : m_FloatingDlgList)
  {
    if (pFloatingDlg != nullptr && pFloatingDlg->GetSafeHwnd() == hFocusingWindow)
    {
      return pFloatingDlg;
    }
  }

  return nullptr;
}

CFloatingDialog* CFloatingDialogManager::GetpUnderMouseDialog()
{
  CFloatingDialog* result = nullptr;

  POINT point = { 0 };
  GetCursorPos(&point);
  HWND hUnderMouseWindow = WindowFromPoint(point);

  if (auto pWnd = CWnd::FromHandle(hUnderMouseWindow))
  {
    if (auto pImageView = dynamic_cast<CImageView*>(CWnd::FromHandle(hUnderMouseWindow)))
    {
      if (auto pParent = pImageView->GetParent())
      {
        hUnderMouseWindow = pParent->GetSafeHwnd();
      }
    }
  }

  for (auto& pFloatingDlg : m_FloatingDlgList)
  {
    if (pFloatingDlg != nullptr && pFloatingDlg->GetSafeHwnd() == hUnderMouseWindow)
    {
      return pFloatingDlg;
    }
  }

  return nullptr;
}
