#pragma once

#include "Visual.h"
#include "EventArgs.h"
#include "EventHandler.h"

namespace Yupei
{
	class UIElement : public virtual Visual
	{
	protected:
		union PosType
		{
			struct forCanvas
			{
				float xPos, yPos;
			} canvasPos;
			struct forGrid
			{
				int row, column;
			} gridPos;
		};
	public:
		/*Events*/
		EventHandler<ControlClickArgs> MouseDown;
		EventHandler<ControlClickArgs> MouseUp;

		UIElement()
			:Visual(),
			finalMatrix(new Graphics::Matrix3x2())
		{
			
		}
		UIElement(float _width,float _height)
			:Visual(),
			width(_width),
			height(_height),
			leftCornerPos(),
			finalMatrix(new Graphics::Matrix3x2())
		{
			
		}
		/*LeftCorner Functions*/
		Graphics::Point GetLeftCorner() const noexcept
		{
			return leftCornerPos;
		}
		void SetLeftCorner(const Graphics::Point& p) noexcept
		{
			leftCornerPos = p;
		}
		void SetLeftCornerX(float _x)
		{
			leftCornerPos.x = _x;
		}
		void SetLeftCornerY(float _y)
		{
			leftCornerPos.y = _y;
		}
		float GetLeftCornerX() const noexcept
		{
			return leftCornerPos.x;
		}
		float GetLeftCornerY() const noexcept
		{
			return leftCornerPos.y;
		}

		void SetWidth(float _width)
		{
			width = _width;
		}
		float GetWidth() const noexcept
		{
			return width;
		}
		void SetHeight(float _height) noexcept
		{
			height = _height;
		}
		float GetHeight() const noexcept
		{
			return height;
		}
		void SetActualWidth(float _width)
		{
			actualWidth = _width;
		}
		float GetActualWidth() const noexcept
		{
			return actualWidth;
		}
		void SetActualHeight(float _height) noexcept
		{
			actualHeight = _height;
		}
		float GetActualHeight() const noexcept
		{
			return actualHeight;
		}

 		float GetCanvasX() const noexcept
		{
			return myPos.canvasPos.xPos;
		}
		void SetCanvasX(float x) noexcept
		{
			myPos.canvasPos.xPos = x;
		}
		float GetCanvasY() const noexcept
		{
			return myPos.canvasPos.yPos;
		}
		void SetCanvasY(float y) noexcept
		{
			myPos.canvasPos.yPos = y;
		}
		int GetGridRow() const noexcept
		{
			return myPos.gridPos.row;
		}
		void SetGridRow(int _row) noexcept
		{
			myPos.gridPos.row = _row;
		}
		int GetGridColumn() const noexcept
		{
			return myPos.gridPos.column;
		}
		void SetGridColumn(int _column) noexcept
		{
			myPos.gridPos.column = _column;
		}
		UIElement* GetLogicalParent() const noexcept
		{
			return logicalParent;
		}
		void SetLogicalParent(UIElement* _parent) noexcept
		{
			logicalParent = _parent;
		}
		virtual void OnResize(bool isIgnored)
		{
			LayoutInternal(isIgnored);
		}
		//���������Լ���ȡ�߶ȡ����ʱ��UIElement��Ĭ����Ϊ�Ƿ����Լ��ĸ߶ȡ����
		//���Ƕ��ڲ�ͬPanel�����ͬ��
		//��ContentControl��Ĭ����Ϊ������ϮUIElement
		//������Grid������Ҫ������߶ȡ����
		virtual float GetChildWidth(UIElement* _child) const noexcept
		{
			return GetActualWidth();
		}
		virtual float GetChildHeight(UIElement* _child) const noexcept
		{
			return GetActualHeight();
		}
		Graphics::Rect GetVirtualPosition() const noexcept
		{
			return Graphics::Rect(0.f,0.f, actualWidth, actualHeight);
		}
		Graphics::Rect GetRealPosition() const noexcept
		{
			return Graphics::Rect(leftCornerPos, actualWidth, actualHeight);
		}
		Graphics::Point GetCentrePoint() const noexcept
		{
			return Graphics::Point{ actualWidth / 2.f,actualHeight / 2.f };
		}
		PosType GetMyPos() const noexcept
		{
			return myPos;
		}
		void OnRender(Graphics::IDrawingContext* dc) override
		{
			FixMatrix();
			//������֮ǰ�ľ���
			auto tempTrans = dc->GetTransform();
			//�����ڻ��Ƶľ�������Ϊ�¾������а�����ƽ��LeftCorner��ʹ�ú��ӿ���ʹ��������꣩��Ӧ��renderTransform��
			dc->SetTransform(finalMatrix);
			
			dc->PushAxisAlignedClip(Graphics::Rect{ 0.f,0.f,actualWidth,actualHeight });
			RenderInternal(dc);
			dc->PopAxisAlignedClip();
			//��ԭ����
			dc->SetTransform(&tempTrans);
		}
		virtual ~UIElement()
		{
			delete finalMatrix;
			finalMatrix = nullptr;
		}
		void SetRenderTransform(Graphics::TransformBase* _trans) noexcept
		{
			renderTransform = _trans;
		}
		void SetLayoutTransform(Graphics::TransformBase* _trans) noexcept
		{
			layoutTransform = _trans;
		}
		float GetDesiredWidth() const noexcept;
		float GetDesiredHeight() const noexcept;
		Graphics::Size GetDesiredSize() const noexcept
		{
			if (layoutTransform != nullptr)
			{
				auto leftTop = leftCornerPos;
				auto rightBottom = Graphics::Point{ GetLeftCornerX() + actualWidth ,GetLeftCornerY() + actualHeight };
				auto leftButtom = Graphics::Point{ GetLeftCornerX(),GetLeftCornerY() + actualHeight };
				auto rightTop = Graphics::Point{ GetLeftCornerX() + actualWidth,GetLeftCornerY() };
				leftTop = layoutTransform->Get()->TransformPoint(leftTop);
				rightBottom = layoutTransform->Get()->TransformPoint(rightBottom);
				leftButtom = layoutTransform->Get()->TransformPoint(leftButtom);
				rightTop = layoutTransform->Get()->TransformPoint(rightTop);
				
			}

		}
	protected:
		virtual void LayoutInternal(bool isIgnored)
		{
			if (!isIgnored)
			{
				ProcessWidth();
				ProcessHeight();
			}
		}
		Graphics::Matrix3x2* GetFinalMatrix() const noexcept
		{
			return finalMatrix;
		}
		//��ΪUIElement����OnRender����ಽ�������Ƶģ��绺�����PushClip���ָ�����ȣ�����ʹ��ģ�巽����
		virtual void RenderInternal(Graphics::IDrawingContext* dc) = 0;

		Graphics::TransformBase* renderTransform = nullptr;
		Graphics::TransformBase* layoutTransform = nullptr;
		Graphics::Point leftCornerPos;
		float width = NAN;
		float height = NAN;
		float actualWidth = NAN;
		float actualHeight = NAN;
		UIElement* logicalParent = nullptr;
		HorizontalAlignmentType horizontalAlignment = HorizontalAlignmentType::Stretch;
		VerticalAlignmentType verticalAlignment = VerticalAlignmentType::Stretch;
		
	private:
		void FixMatrix() noexcept
		{
			FixMatrixInternal(layoutTransform);
			//FixMatrixInternal(renderTransform);
		}
		void FixMatrixInternal(Graphics::TransformBase* trans)
		{
			if (trans)
			{
				finalMatrix->SetProduct(Graphics::Matrix3x2::Translation(
					leftCornerPos.x, leftCornerPos.y), *trans->Get());
			}
			else
			{
				*finalMatrix = Graphics::Matrix3x2::Translation(
					leftCornerPos.x, leftCornerPos.y);
			}
		}
		void ProcessWidth() noexcept;
		void ProcessHeight() noexcept;

		Graphics::Matrix3x2* finalMatrix;
		PosType myPos;
		bool isMouseOver = false;
		bool isClicking = false;
		bool isVisible = false;
		bool isEnabled = true;
	};
}
