#pragma once

#include "Common.h"
#include "GraphicsBasis.h"

namespace Yupei
{
	enum class EventTypes
	{
		WindowCreate,
		WindowPaint,// so on
	};

	//EventArguments�Ĺ淶��
	//1.������һ��Ĭ�Ϲ��캯����Ĭ�Ϲ��캯�����ú�֤��Աȫ��Ĭ�ϳ�ʼ��
	//2.������һ������Object*�����Ĺ��캯��
	//3.�����и���Helper����������wParam/lParam�����������ת��Ϊ���ڲ���Ա
	//4.������ʹ��Ĭ�ϲ���
	struct EventArguements
	{
		EventArguements(){}
		EventArguements(Object* s) { source = s; }
		Object* source = nullptr;
		bool isHandled = false;
		//EventTypes eventType;
	};

	struct WindowCreateArgs : public EventArguements
	{
		WindowCreateArgs() {}
		WindowCreateArgs(Object* s)
			:EventArguements(s)
		{
		}
		WindowCreateArgs(Object* s, CREATESTRUCT* structPtr)
			:EventArguements(s)
		{
			createStructPtr = structPtr;
		}
		
		CREATESTRUCT* createStructPtr = nullptr;
	};

	struct WindowPaintArgs :public EventArguements
	{
		WindowPaintArgs() {}
		WindowPaintArgs(Object* s)
			:EventArguements(s)
		{
		}
		WindowPaintArgs(Object* s, Graphics::IDrawingContext* _context)
			:EventArguements(s)
		{
			context = _context;
		}
		
		Graphics::IDrawingContext* context = nullptr;
	};

	struct WindowNCPaintArgs :public EventArguements
	{
		WindowNCPaintArgs(){}
		WindowNCPaintArgs(Object* s)
			:EventArguements(s)
		{

		}
		WindowNCPaintArgs(Object* s, WPARAM rgn)
			:EventArguements(s),regionHandle((HRGN)rgn)
		{
			
		}
		HRGN regionHandle = nullptr;
		
	};

	struct WindowResizeArgs :public EventArguements
	{
		WindowResizeArgs() {}
		WindowResizeArgs(Object* s)
			:EventArguements(s)
		{
		}
		WindowResizeArgs(Object* s, WPARAM w,LPARAM l)
			:EventArguements(s),newSize(LOWORD(l),HIWORD(l))
		{
			resizeType = w;
		}
		//Helper Function
		void SetSize(LPARAM l)
		{
			newSize = { static_cast<float>(LOWORD(l)),
				static_cast<float>(HIWORD(l)) };
		}
		Graphics::Size newSize;
		WPARAM resizeType = 0;
	};

	struct ControlClickArgs : public EventArguements
	{
		ControlClickArgs() = default;
		ControlClickArgs(Object* s)
			:EventArguements(s)
		{
		}
		ControlClickArgs(Object* s,const Graphics::Point& point)
			:EventArguements(s),clickPos(point)
		{
		}
		ControlClickArgs(const Graphics::Point& point)
			:ControlClickArgs(nullptr,point)
		{
		}
		Graphics::Point clickPos;
	};
	//struct ControlResizeArgs : public EventArguements
	//{
	//	ControlResizeArgs() = default;
	//	ControlResizeArgs(Object* s)
	//		:EventArguements(s)
	//	{
	//	}
	//	ControlResizeArgs(Object* s, const Graphics::Rect& _size)
	//		:EventArguements(s), nowRect(_size)
	//	{
	//	}
	//	ControlResizeArgs(Object* s, WPARAM w, LPARAM l)
	//		:EventArguements(s), 
	//		nowRect(0.f,0.f,static_cast<float>(LOWORD(l)), static_cast<float>(HIWORD(l)))
	//	{
	//	}
	//	ControlResizeArgs(const Graphics::Rect& _size)
	//		:ControlResizeArgs(nullptr, _size)
	//	{
	//	}
	//	//Helper Function
	//	void SetSize(LPARAM l)
	//	{
	//		nowRect = {0.f,0.f, static_cast<float>(0.f,0.f,LOWORD(l)), static_cast<float>(HIWORD(l)) };
	//	}
	//	Graphics::Rect nowRect;//the rect of itself
	//};
}
