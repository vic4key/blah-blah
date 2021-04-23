/**
 * Window GUI - Iterate all child controls inside a parent like dialog, etc.
 */

auto fn_iterate_control = [](const HWND& hWndParent, const std::function<bool(const HWND& hWnd)> fn)
{
  for (auto hWnd = ::GetWindow(hWndParent, GW_CHILD); hWnd != nullptr; hWnd = ::GetNextWindow(hWnd, GW_HWNDNEXT))
  {
    if (!fn(hWnd)) break;
  }
};

auto fn_iterate_control_mfc = [](const CWnd* pParent, const std::function<bool(CWnd& child)> fn)
{
  for (auto pChild = pParent->GetWindow(GW_CHILD); pChild != nullptr; pChild = pChild->GetNextWindow())
  {
		if (!fn(*pChild)) break;
  }
};

// fn_iterate_control(this->GetSafeHwnd(), [](const HWND& hWnd) -> bool
// fn_iterate_control_mfc(this, [](CWnd& child) -> bool
{
	vu::tchar cn[MAXBYTE] = { 0 };
	::GetClassName(hWnd, cn, ARRAYSIZE(cn));

	vu::tchar wt[MAXBYTE] = { 0 };
	::GetWindowText(hWnd, wt, ARRAYSIZE(wt));

	vu::Msg(_T("%08X | `%s` | `%s`"), hWnd, cn, wt);

	return true;
});