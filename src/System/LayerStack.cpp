#include "LayerStack.h"

namespace Skynet 
{
    LayerStack::~LayerStack() {
        for (Layer* layer : layers) {
			layer->OnDetach();
			delete layer;
		}
    }

    void LayerStack::PushLayer(Layer* layer) {
        layers.emplace(layers.begin() + index, layer);
		index++;
    }

    void LayerStack::PushOverLayer(Layer* overlayer) {
        layers.emplace_back(overlayer);
    }

    void LayerStack::PopLayer(Layer* layer) {
        auto it = std::find(layers.begin(), layers.begin() + index, layer);
		if (it != layers.begin() + index) {
			layer->OnDetach();
			layers.erase(it);
			index--;
		}
    }

    void LayerStack::PopOverLayer(Layer* overlayer) {
		auto it = std::find(layers.begin() + index, layers.end(), overlayer);
		if (it != layers.end()) {
			overlayer->OnDetach();
			layers.erase(it);
		}
	}

} //namespace Skynet 