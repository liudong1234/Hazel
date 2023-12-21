#include "hzpch.h"
#include "LayerStack.h"


namespace Hazel
{
	LayerStack::LayerStack()
	{
		this->m_LayerInsertIndex = 0;
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : this->m_Layers)
		{
			if (layer != nullptr)
				delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		this->m_Layers.emplace(this->m_Layers.begin() + this->m_LayerInsertIndex, layer);
		this->m_LayerInsertIndex++;
	}

	void LayerStack::PushOverLayer(Layer* overLayer)
	{
		this->m_Layers.emplace_back(overLayer);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto it = std::find(this->m_Layers.begin(), this->m_Layers.end(), layer);
		if (it != this->m_Layers.end())
		{
			this->m_Layers.erase(it);
			m_LayerInsertIndex--;
		}
	}
	void LayerStack::PopOverLayer(Layer* layer)
	{
		auto it = std::find(this->m_Layers.begin(), this->m_Layers.end(), layer);
		if (it != this->m_Layers.end())
		{
			this->m_Layers.erase(it);
		}
	}
}