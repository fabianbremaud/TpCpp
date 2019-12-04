#include "pch.h"
#include "CElement.h"
#include "ElementFactory.h"
#include "MainFrm.h"
#include "DrawingContext.h"
#include "DrawingElements.h"

// 
// CShapeType
//
ShapeType CShapeType::ToShapeType(int value)
{
	ShapeType ret = ShapeType::unknown;
	switch (value)
	{
	case line:
	case line_right:
	case rectangle:
	case ellipse:
	case rectangle_round:
	case triangle:
	case line_broken:
	case line_broken_right:
	case notimp_igloo:
	case notimpl_multi_line:
	case arrow_right:
	case arrow_down:
	case line2:
	case line_right2:
	case line_left_right:
	case parenthese_left:
	case parenthese_right:
	case star:
		ret = (ShapeType)value;
		break;
	case image_fixed:
	case image_scaled:
		ret = (ShapeType)value;
		break;
	case infrastructure_server_ad:
	case infrastructure_server:
	case infrastructure_server_web:
	case infrastructure_server_database:
	case infrastructure_workstation:
	case infrastructure_laptop:
	case infrastructure_firewall:
	case infrastructure_network:
	case infrastructure_virtual_server:
	case infrastructure_virtual_server_web:
	case infrastructure_virtual_server_database:
	case infrastructure_virtualization_server:
	case infrastructure_server_ad_grey:
	case infrastructure_server_grey:
	case infrastructure_server_database_grey:
	case infrastructure_server_farm:
	case infrastructure_workstation_grey:
	case infrastructure_laptop_grey:
		ret = (ShapeType)value;
		break;
	case text:
	case text_left:
	case text_center:
	case text_right:
	case text_justify:
		ret = (ShapeType)value;
		break;
	case development_class:
	case development_interface:
	case development_enumeration:
	case development_package:
	case development_comment:
	case development_association:
	case development_aggregation:
	case development_composition:
	case development_dependency:
	case development_inheritance:
	case development_package_import:
	case development_connector:
	case development_component:
		ret = (ShapeType)value;
		break;
	default: break;
	}
	return ret;
}

//
// CElement Class
//

IMPLEMENT_SERIAL(CElement, CObject, 0)

int CElement::m_counter = 0;

CElement::CElement()
{
	m_pManager = NULL;
	m_pView = NULL;

	m_shapeType = ShapeType::unknown;
	m_rect.SetRectEmpty();
	m_point = CPoint(0, 0);
	m_last = m_point;
	m_text = L"";
	m_bColorFill = true;
	m_colorFill = RGB(154, 200, 249);
	m_bSolidColorFill = false;
	m_bColorLine = true;
	m_colorLine = RGB(0, 0, 0);
	m_bLineWidth = true;
	m_lineWidth = 1;
	m_image = L"";
	m_fontName = L"Calibri";
	m_fontSize = 12;

	m_bMoving = FALSE;
	m_type = ElementType::type_unknown;
	m_shapeType = ShapeType::unknown;

	SetGuid();
	SetName();
}

CElement::~CElement(void)
{
}

void CElement::SetGuid()
{
	USES_CONVERSION;

	m_counter++;

	CGuid guid;
#ifdef GUID_TRUE
	m_objectId = guid.ToString();
#else
	SYSTEMTIME st;
	GetSystemTime(&st);
	CTime dt = CTime::GetCurrentTime();
	CString objectId;
	objectId.Format(_T("guid_%d_%s_%03d_%ld"), m_counter, dt.Format(_T("%H%M%S")), st.wMilliseconds, GetTickCount());
	this->m_objectId = T2W((LPTSTR)(LPCTSTR)objectId);
#endif
}

void CElement::SetName()
{
	USES_CONVERSION;

	// Inc counter of created objects
	CFactory::g_counter++;
	// element name
	CTime dt = CTime::GetCurrentTime();
	CString name;
	name.Format(_T("Element n_%d"), CFactory::g_counter); //_%s.%03d", dt.Format("%H%M%S"), tm.wMilliseconds);
	this->m_name = T2W((LPTSTR)(LPCTSTR)name);
}

std::shared_ptr<CElement> CElement::MakeCopy()
{
	std::shared_ptr<CElement> pNewElement = CFactory::CreateElementOfType((ElementType)this->m_type,
		(ShapeType)this->m_shapeType);
	pNewElement->m_name = this->m_name;
	pNewElement->m_objectId = this->m_objectId;
	pNewElement->m_caption = this->m_caption;
	pNewElement->m_text = this->m_text;
	pNewElement->m_lineWidth = this->m_lineWidth;
	pNewElement->m_pManager = this->m_pManager;
	pNewElement->m_pView = this->m_pView;
	pNewElement->m_rect = this->m_rect;
	pNewElement->m_bColorFill = m_bColorFill;
	pNewElement->m_bColorLine = m_bColorLine;
	pNewElement->m_bLineWidth = m_bLineWidth;
	pNewElement->m_bSolidColorFill = m_bSolidColorFill;
	pNewElement->m_colorFill = m_colorFill;
	pNewElement->m_colorLine = m_colorLine;

	return pNewElement;
}

void CElement::Serialize(CArchive& ar)
{
	USES_CONVERSION;

	if (ar.IsStoring())
	{
		CString name = W2T((LPTSTR)m_name.c_str());
		ar << name;
		int type = m_type;
		ar << type;
		int shapeType = m_shapeType;
		ar << shapeType;
		CString objectId = W2T((LPTSTR)m_objectId.c_str());
		ar << objectId;
		CString caption = W2T((LPTSTR)m_caption.c_str());
		ar << caption;
		CString text = W2T((LPTSTR)m_text.c_str());
		ar << text;
		ar << m_rect;
		ar << m_point;
		ar << m_last;
		ar << m_colorFill;
		ar << m_bColorFill;
		ar << m_colorLine;
		ar << m_bColorLine;
		ar << m_bColorLine;
		ar << m_lineWidth;
		CString image = W2T((LPTSTR)m_image.c_str());
		ar << image;
	}
	else
	{
		// get the document back pointer from the archive
		CModeler1Doc* pDocument = (CModeler1Doc*)ar.m_pDocument;
		m_pManager = pDocument->GetManager();

		CString name;
		ar >> name;
		this->m_name = T2W((LPTSTR)(LPCTSTR)name);
		int type;
		ar >> type;
		m_type = (ElementType)type;
		int shapeType;
		ar >> shapeType;
		m_shapeType = (ShapeType)shapeType;
		CString objectId;
		ar >> objectId;
		this->m_objectId = T2W((LPTSTR)(LPCTSTR)objectId);
		CString caption;
		ar >> caption;
		this->m_caption = T2W((LPTSTR)(LPCTSTR)caption);
		CString text;
		ar >> text;
		this->m_text = T2W((LPTSTR)(LPCTSTR)text);
		ar >> m_rect;
		ar >> m_point;
		ar >> m_last;
		ar >> m_colorFill;
		ar >> m_bColorFill;
		ar >> m_colorLine;
		ar >> m_bColorLine;
		ar >> m_bColorLine;
		ar >> m_lineWidth;
		CString image;
		ar >> image;
		this->m_image = T2W((LPTSTR)(LPCTSTR)image);
	}
}

CString CElement::ToString()
{
	CString str;
	str.Format(_T("Element name=<%s> id={%s} type=<%s> shape=<%s> rect={%d,%d,%d,%d} caption=<%s> text=<%s> image=<%s> colorFill={%03d%03d%03d} colorLine={%03d%03d%03d}"),
		m_name.c_str(), m_objectId.c_str(), ToString(m_type), ToString(m_shapeType),
		m_rect.left, m_rect.top, m_rect.right, m_rect.bottom,
		m_caption.c_str(), m_text.c_str(), m_image.c_str(),
		GetRValue(m_colorFill), GetGValue(m_colorFill), GetBValue(m_colorFill),
		GetRValue(m_colorLine), GetGValue(m_colorLine), GetBValue(m_colorLine));
	return str;
}

CString CElement::ToString(ElementType type)
{
	CString str = _T("");
	switch (type)
	{
	case ElementType::type_unknown:
		str = _T("type_unknown->not implemented yet");
		break;

	case ElementType::type_shapes_simple:
		str = _T("type_shapes_simple");
		break;

	case ElementType::type_image:
		str = _T("type_image");
		break;

	case ElementType::type_shapes_infrastructure:
		str = _T("type_shapes_infrastructure");
		break;

	case ElementType::type_text:
		str = _T("type_text");
		break;

	case ElementType::type_shapes_development:
		str = _T("type_shapes_development");
		break;

	default:
		break;
	}
	return str;
}

CString CElement::ToString(ShapeType type)
{
	CString str = _T("");
	switch (type)
	{
	case line:
		str = _T("line");
		break;
	case line_right:
		str = _T("line_right");
		break;
	case rectangle:
		str = _T("rectangle");
		break;
	case ellipse:
		str = _T("ellipse");
		break;
	case rectangle_round:
		str = _T("rectangle_round");
		break;
	case triangle:
		str = _T("triangle");
		break;
	case line_broken:
		str = _T("line_broken");
		break;
	case line_broken_right:
		str = _T("line_broken_right");
		break;
	case line2:
		str = _T("line2");
		break;
	case line_right2:
		str = _T("line_right2");
		break;
	case line_left_right:
		str = _T("line_left_right");
		break;
	case arrow_right:
		str = _T("arrow_right");
		break;
	case arrow_down:
		str = _T("arrow_down");
		break;
	case parenthese_left:
		str = _T("parenthese_left");
		break;
	case parenthese_right:
		str = _T("parenthese_right");
		break;
	case star:
		str = _T("star");
		break;

	case image_fixed:
		str = _T("image_fixed");
		break;
	case image_scaled:
		str = _T("image_scaled");
		break;

	case infrastructure_server_ad:
		str = _T("infrastructure_server_ad");
		break;
	case infrastructure_server:
		str = _T("infrastructure_server");
		break;
	case infrastructure_server_web:
		str = _T("infrastructure_server_web");
		break;
	case infrastructure_server_database:
		str = _T("infrastructure_server_web");
		break;
	case infrastructure_workstation:
		str = _T("infrastructure_workstation");
		break;
	case infrastructure_laptop:
		str = _T("infrastructure_laptop");
		break;
	case infrastructure_firewall:
		str = _T("infrastructure_firewall");
		break;
	case infrastructure_network:
		str = _T("infrastructure_network");
		break;
	case infrastructure_virtual_server:
		str = _T("infrastructure_virtual_server");
		break;
	case infrastructure_virtual_server_web:
		str = _T("infrastructure_virtual_server_web");
		break;
	case infrastructure_virtual_server_database:
		str = _T("infrastructure_virtual_server_database");
		break;
	case infrastructure_virtualization_server:
		str = _T("infrastructure_virtualization_server");
		break;
	case infrastructure_server_ad_grey:
		str = _T("infrastructure_server_ad");
		break;
	case infrastructure_server_grey:
		str = _T("infrastructure_server");
		break;
	case infrastructure_server_database_grey:
		str = _T("infrastructure_server_database");
		break;
	case infrastructure_server_farm:
		str = _T("infrastructure_server_farm");
		break;
	case infrastructure_workstation_grey:
		str = _T("infrastructure_workstation");
		break;
	case infrastructure_laptop_grey:
		str = _T("infrastructure_laptop");
		break;

	case text:
		str = _T("text");
		break;
	case text_left:
		str = _T("text_left");
		break;
	case text_center:
		str = _T("text_center");
		break;
	case text_right:
		str = _T("text_right");
		break;
	case text_justify:
		str = _T("text_justify");
		break;

	case development_class:
		str = _T("development_class");
		break;
	case development_interface:
		str = _T("development_interface");
		break;
	case development_enumeration:
		str = _T("development_enumeration");
		break;
	case development_package:
		str = _T("development_package");
		break;
	case development_comment:
		str = _T("development_comment");
		break;
	case development_association:
		str = _T("development_association");
		break;
	case development_aggregation:
		str = _T("development_aggregation");
		break;
	case development_composition:
		str = _T("development_composition");
		break;
	case development_dependency:
		str = _T("development_dependency");
		break;
	case development_inheritance:
		str = _T("development_inheritance");
		break;
	case development_package_import:
		str = _T("development_package_import");
		break;
	case development_connector:
		str = _T("development_connector");
		break;
	case development_component:
		str = _T("development_component");
		break;
	}
	return str;
}

/*
bool CElement::IsDrawable(ElementType type)
{
	bool ret = false;
	switch(type)
	{
		case ElementType::type_rectangle:
		case ElementType::type_class:
		case ElementType::type_line:
		case ElementType::type_ellipse:
		case ElementType::type_image:
		case ElementType::type_shapes:
			ret = true;
		break;
	}
	return ret;
}
*/

bool CElement::CanChangeFillColor() const
{
	return true;
}

bool CElement::CanChangeLineColor() const
{
	return true;
}

bool CElement::CanChangeLineWidth() const
{
	return true;
}

bool CElement::Intersects(const CRect& rect)
{
	CRect fixed = m_rect;
	CSize addSize(5, 5);
	fixed.InflateRect(addSize);
	fixed.NormalizeRect();
	CRect rectT = rect;
	rectT.NormalizeRect();

	// Debug
#ifdef DRAW_EXTERNAL_RECTANGLE_HITS
	//CClientDC dc(this->m_pView);
	//Graphics graphics(dc.m_hDC);
	//
	//Color colorBlack(255, 0, 0, 0);
	//Pen penBlack(colorBlack);

	//CRect rect1 = fixed;
	//CRect rect2 = rectT;
	//CRect rect3 = rect1 & rect2;

	//GetManager()->ManagerToView(this->m_pView, rect1);
	//GetManager()->ManagerToView(this->m_pView, rect2);
	//GetManager()->ManagerToView(this->m_pView, rect3);
	//graphics.DrawRectangle(&penBlack, rect1.left, rect1.top, rect1.Width(), rect1.Height());
	//graphics.DrawRectangle(&penBlack, rect2.left, rect2.top, rect2.Width(), rect2.Height());
	//graphics.DrawRectangle(&penBlack, rect3.left, rect3.top, rect3.Width(), rect3.Height());
#endif

	return !(rectT & fixed).IsRectEmpty();
}

bool CElement::IsLine()
{
	if (m_shapeType == ShapeType::line || m_shapeType == ShapeType::line2 ||
		m_shapeType == ShapeType::line_left_right ||
		m_shapeType == ShapeType::line_broken || m_shapeType == line_broken_right ||
		m_shapeType == ShapeType::line_right || m_shapeType == ShapeType::line_right2)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CElement::InvalidateObj(void)
{
	this->m_rect.SetRect(this->m_point, this->m_last);
	if (IsLine())
	{
	}
	else
	{
		// rectangle
		this->m_rect.NormalizeRect();
	}
}

void CElement::CheckForKeepingAMinimumSize()
{
	const int defaultSize = 10;

	// For all types
	if (m_type == type_shapes_simple || m_type == type_image) //m_type == type_shapes_infrastructure ) //||  )
	{
		if (m_rect.Width() < defaultSize && m_rect.Height() < defaultSize)
		{
			CPoint p1 = m_rect.TopLeft();
			CPoint p2;
			p2.x = p1.x + defaultSize;
			p2.y = p1.y + defaultSize;
			m_rect.SetRect(p1, p2);
		}
	}
}

// Selection & Tracker helpers

int CElement::GetHandleCount()
{
	if (IsLine())
	{
		return 2; //8;
	}
	else
		return 8;
}

int CElement::HitTest(CPoint point, CModeler1View* pView, bool bSelected)
{
	CString str;
	if (bSelected)
	{
		int nHandleCount = GetHandleCount();
		for (int nHandle = 1; nHandle <= nHandleCount; nHandle += 1)
		{
			// GetHandleRect returns in logical coords
			CRect rc = GetHandleRect(nHandle, pView);

			// Debugging
			//str.Format(_T("HitTest try point{%d,%d} in rect{%d,%d,%d,%d}"), point.x, point.y, rc.left, rc.top, rc.right, rc.bottom);
			//pView->LogDebug(str);

			CRect rect(point, CSize(1, 1));
			CRect fixed = rc;
			CSize addSize(5, 5);
			fixed.InflateRect(addSize);
			fixed.NormalizeRect();
			if ((rect & fixed).IsRectEmpty() == FALSE)
			{
				// Debugging
				//str.Format("HitTest returns %d", nHanlde);
				//pView->LogDebug(str);
				return nHandle;
			}

			//if (point.x >= rc.left && point.x < rc.right && point.y <= rc.top && point.y > rc.bottom)
			//	return nHandle;
		}
	}
	else
	{
		if (point.x >= m_rect.left && point.x < m_rect.right && point.y <= m_rect.top && point.y > m_rect.bottom)
			return 1;
	}

	return 0;
}

// return rectange of handle in logical coords
CRect CElement::GetHandleRect(int nHandleID, CModeler1View* pView)
{
	CRect rect;

	// get the center of the handle in logical coords
	CPoint point = GetHandle(nHandleID);
	// convert to client/device coords
	GetManager()->ManagerToView(pView, point);
	//pView->DocToClient(point);
	// return CRect of handle in device coords
	rect.SetRect(point.x - 3, point.y - 3, point.x + 3, point.y + 3);
	//pView->ClientToDoc(rect);
	GetManager()->ViewToManager(pView, rect);

	return rect;
}

// returns logical coords of center of handle
CPoint CElement::GetHandle(int nHandle)
{
	// lines have only two dots tracker !
	if (IsLine() && nHandle == 2)
		nHandle = 5;

	int x, y, xCenter, yCenter;
	CRect rect = m_rect;

	// this gets the center regardless of left/right and top/bottom ordering
	xCenter = rect.left + rect.Width() / 2;
	yCenter = rect.top + rect.Height() / 2;

	switch (nHandle)
	{
	default:
		ASSERT(FALSE);

	case 1:
		x = rect.left;
		y = rect.top;
		break;

	case 2:
		x = xCenter;
		y = rect.top;
		break;

	case 3:
		x = rect.right;
		y = rect.top;
		break;

	case 4:
		x = rect.right;
		y = yCenter;
		break;

	case 5:
		x = rect.right;
		y = rect.bottom;
		break;

	case 6:
		x = xCenter;
		y = rect.bottom;
		break;

	case 7:
		x = rect.left;
		y = rect.bottom;
		break;

	case 8:
		x = rect.left;
		y = yCenter;
		break;
	}

	return CPoint(x, y);
}

HCURSOR CElement::GetHandleCursor(int nHandle)
{
	// lines have only two dots tracker !
	if (IsLine() && nHandle == 2)
		nHandle = 5;

	LPCTSTR id;
	switch (nHandle)
	{
	case 1:
	case 5:
		id = IDC_SIZENWSE;
		break;

	case 2:
	case 6:
		id = IDC_SIZENS;
		break;

	case 3:
	case 7:
		id = IDC_SIZENESW;
		break;

	case 4:
	case 8:
		id = IDC_SIZEWE;
		break;

	default:
		id = IDC_ARROW;
		//ASSERT(FALSE);
		break;
	}

	return AfxGetApp()->LoadStandardCursor(id);
}

// point must be in logical
void CElement::MoveHandleTo(int nHandle, CPoint point, CModeler1View* pView)
{
	// lines have only two dots tracker !
	if (IsLine() && nHandle == 2)
		nHandle = 5;

	CRect position = m_rect;
	switch (nHandle)
	{
	case 1:
		position.left = point.x;
		position.top = point.y;
		break;

	case 2:
		position.top = point.y;
		break;

	case 3:
		position.right = point.x;
		position.top = point.y;
		break;

	case 4:
		position.right = point.x;
		break;

	case 5:
		position.right = point.x;
		position.bottom = point.y;
		break;

	case 6:
		position.bottom = point.y;
		break;

	case 7:
		position.left = point.x;
		position.bottom = point.y;
		break;

	case 8:
		position.left = point.x;
		break;

	default:
		ASSERT(FALSE);
	}

	m_rect = position;

}

void CElement::DrawTracker(CDrawingContext& ctxt, TrackerState state)
{
	Color colorBlack(255, 0, 0, 0);
	SolidBrush solidBrush(colorBlack);
	CRect rect = m_rect;

	switch (state)
	{
	case normal:
		break;

	case selected:
	case active:
	{
		int nHandleCount = GetHandleCount();
		for (int nHandle = 1; nHandle <= nHandleCount; nHandle += 1)
		{
			CPoint handle = GetHandle(nHandle);
			//pDC->PatBlt(handle.x - 3, handle.y - 3, 7, 7, DSTINVERT);
			ctxt.GetGraphics()->FillRectangle(&solidBrush, handle.x - 3, handle.y - 3, 7, 7);
		}
	}
	break;
	}
}

std::wstring CElement::GetImageFilePath()
{
	std::wstring ret = L"";

	// FIXME
	std::wstring strShapePath = L"Images\\Shapes\\";
	if (this->m_type == ElementType::type_shapes_infrastructure)
	{
		ret = strShapePath + m_image;
	}
	else
	{
		ret = m_image;
	}
	return ret;
}

void CElement::Draw(CDrawingContext& ctxt)
{
	// Nothing to implement
	Color colorBlack(255, 0, 0, 0);
	Pen penBlack(colorBlack);
	CRect rect = m_rect;
	ctxt.GetGraphics()->DrawRectangle(&penBlack, rect.left, rect.top, rect.Width(), rect.Height());

	// Debug Only
	CString str;
	str.Format(_T("CElement::Draw()"));
	m_pView->LogDebug(str);
	//AfxMessageBox(str);
}

void CallDraw(CDrawingContext& ctxt, std::shared_ptr<CElement> pElement)
{
	pElement->Draw(ctxt);
}

void CElement::Draw(CModeler1View* pView, CDC* pDC)
{
	Graphics graphics(pDC->m_hDC);
	//graphics.ScaleTransform(0.75f, 0.75f);
	graphics.ScaleTransform(GetManager()->m_fZoomFactor, GetManager()->m_fZoomFactor);

	//graphics.SetRenderingOrigin(0, 0);
	//graphics.RotateTransform(45.0);
	//graphics.RotateTransform(5.0f); //, MatrixOrderAppend);  // then rotate

#ifdef DRAW_DIRECLY_A_LINE
	{
		if (m_type == ElementType::type_shapes_simple)
		{
			if (IsLine())
			{
				CRect rect = m_rect;
				//rect.NormalizeRect();
				CPoint p1 = rect.TopLeft();
				CPoint p2 = rect.BottomRight();

				// Color for line
				Color colorLine(255, GetRValue(m_colorLine), GetGValue(m_colorLine), GetBValue(m_colorLine));
				Pen penLine(colorLine);
				graphics.DrawLine(&penLine, p1.x, p1.y, p2.x, p2.y);
				return;
			}
		}
	}
#endif

	CRect rect = m_rect;
	//rect.NormalizeRect();
	CPoint p1 = rect.TopLeft();
	CPoint p2 = rect.BottomRight();
	CPoint pointText(rect.left + 10, rect.top + 10);
	// RectF object for Text
	PointF pointF(p1.x, p1.y);
	SizeF sizeF(rect.Width(), rect.Height());
	RectF rectF(pointF, sizeF);

	Color colorWhite(255, 255, 255, 255);
	// Set gradient color
	Color color1(255, 241, 247, 255);
	Color color2(255, GetRValue(m_colorFill), GetGValue(m_colorFill), GetBValue(m_colorFill));
	// Set the color along the entire boundary of the path to aqua.
	LinearGradientBrush lgBrush(Point(rect.left, rect.top),
		Point(rect.right, rect.bottom),
		color1,
		color2);
	//Pen pen(&lgBrush);
	Color colorBlack(255, 0, 0, 0);
	Pen penBlack(colorBlack);
	// Color for line
	Color colorLine(255, GetRValue(m_colorLine), GetGValue(m_colorLine), GetBValue(m_colorLine));
	Pen penLine(colorLine);
	// Color for and Width
	int m_width = m_lineWidth;// FIXME m_8;
	Pen colorPen(colorLine, m_width);
	// Color for fill
	Color colorFill(255, GetRValue(m_colorFill), GetGValue(m_colorFill), GetBValue(m_colorFill));
	SolidBrush solidBrush(colorFill);
	SolidBrush solidBrushWhite(colorWhite);

	
	if (m_text.empty() == false)
	{
		//graphics.FillRectangle(&lgBrush, rect.left, rect.top, rect.Width(), rect.Height());

		// Font object
		FontFamily fontFamily(L"Calibri");
		Gdiplus::Font font(&fontFamily, 12, FontStyleRegular, UnitPixel);
		SolidBrush solidBrush(Color(255, 0, 0, 0));
		graphics.SetTextRenderingHint(TextRenderingHintAntiAlias);
		graphics.DrawString(CStringW(m_text.c_str()), -1, &font, PointF(pointText.x, pointText.y), &solidBrush);
	}
}



