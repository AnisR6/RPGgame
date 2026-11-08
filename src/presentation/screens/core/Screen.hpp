#pragma once

#include "presentation/widgets/core/Widget.hpp"

#include <memory>
#include <stdexcept>
#include <utility>

class Screen {
  protected:
	std::unique_ptr<Widget> rootWidget;

  public:
	explicit Screen(std::unique_ptr<Widget> rootWidget) : rootWidget(std::move(rootWidget))
	{
		if (!this->rootWidget) {
			throw std::invalid_argument("Screen requires a root widget");
		}
	}

	virtual ~Screen() = default;

	[[nodiscard]] Widget &getRootWidget() { return *rootWidget; }

	[[nodiscard]] const Widget &getRootWidget() const { return *rootWidget; }

	void setRootWidget(std::unique_ptr<Widget> newRoot)
	{
		if (!newRoot) {
			throw std::invalid_argument("Screen requires a valid root widget!");
		}
		rootWidget = std::move(newRoot);
	}
};
