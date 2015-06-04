#pragma once

#include "UIElement.h"

namespace Yupei
{
	class Panel : public ContainerVisual, public UIElement
	{
	public:
		Panel()
			:Visual(),ContainerVisual(),UIElement()
		{
		}
		void SetBackground(Graphics::IBrush* brush) noexcept
		{
			backgroundBrush = brush;
		}
		Graphics::IBrush* GetBackground() const noexcept
		{
			return backgroundBrush;
		}
		void AddLogicalChild(UIElement* ui)
		{
			assert(ui->GetLogicalParent() == nullptr);
			ui->SetLogicalParent(this);
			logicalChildren.push_back(ui);
		}
		vector<UIElement*>& GetLogicalChildren()
		{
			return logicalChildren;
		}
		const vector<UIElement*>& GetLogicalChildren() const
		{
			return logicalChildren;
		}
		
		virtual void ArrangeChildren(bool isIgnored)
		{
			for (auto&& _child : GetLogicalChildren())
			{
				_child->OnResize(false);
			}
		}
		bool HitTest(const Graphics::Point& point) const noexcept override
		{
			return GetRealPosition().IsIn(renderTransform->GetOrignalPoint(point));
		}
		/*IsIgnoredΪtrue��ʾ���ܡ��ҡ���HorizonalAlignment*/
		//������Panel�Ƚ��Լ��źð棬������ÿ��child�Լ��Ű�
		//Canvas��ͬ���Լ��źð�������Ը�child�Ű棬��Igorned����Ϊtrue����
		//ÿ��Canvas��Child����IgnoreΪtrue��ʲô����������ΪParent�Ѿ����Լ�
		//�źð���
		void OnResize(bool isIgnored) override
		{
			UIElement::OnResize(isIgnored);
			ArrangeChildren(isIgnored);
		}
		
		void RenderInternal(Graphics::IDrawingContext* context) override;

	private:
		Graphics::IBrush* backgroundBrush = nullptr;
		vector<UIElement*> logicalChildren;
	};

	class Canvas : public Panel
	{
	public:
		static inline void SetX(UIElement* element, float _x) noexcept/*(noexcept(UIElement::SetCanvasX))*/
		{
			element->SetCanvasX(_x);
		}
		static inline void SetY(UIElement* element, float _y) noexcept/*(noexcept(UIElement::SetCanvasY))*/
		{
			element->SetCanvasY(_y);
		}
		float GetChildWidth(UIElement* _child) const noexcept override
		{
			return _child->GetActualWidth();
		}
		float GetChildHeight(UIElement* _child) const noexcept override
		{
			return _child->GetActualHeight();
		}

		void ArrangeChildren(bool isIgnored) override;
	};

	class StackPanel : public Panel
	{
	public:
		enum class OrientationType
		{
			Vertical,
			Horizontal
		};
		float GetChildWidth(UIElement* _child) const noexcept override
		{
			return _child->GetActualWidth();
		}
		float GetChildHeight(UIElement* _child) const noexcept override
		{
			return _child->GetActualHeight();
		}
		void ArrangeChildren(bool isIgnored) override;
	private:
		OrientationType orientetion = OrientationType::Vertical;
	};
}
