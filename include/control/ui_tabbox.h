﻿#pragma once
/**
* Copyright (c) 2014-2018 dustpg   mailto:dustpg@gmail.com
*
* Permission is hereby granted, free of charge, to any person
* obtaining a copy of this software and associated documentation
* files (the "Software"), to deal in the Software without
* restriction, including without limitation the rights to use,
* copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following
* conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
* OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
* HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*/

#include "ui_control.h"
//#include "../util/ui_double_click.h"

// ui namespace
namespace LongUI {
    // tabs
    class UITabs;
    // tabpanels 
    class UITabPanels;
    // tabbox container
    class UITabBox : public UIControl {
        // super class
        using Super = UIControl;
    protected:
        // ctor
        UITabBox(UIControl* parent, const MetaControl&) noexcept;
    public:
        // class meta
        static const  MetaControl   s_meta;
        // dtor
        ~UITabBox() noexcept;
        // ctor
        UITabBox(UIControl* parent = nullptr) noexcept : UITabBox(parent, UITabBox::s_meta) {}
    public:
        // selected changed
        static inline constexpr auto _selectedChanged() noexcept { return GuiEvent::Event_Change; }
    public:
        // set selected index
        void SetSelectedIndex(uint32_t index) noexcept;
        // get selected index
        auto GetSelectedIndex() const noexcept { return m_index; }
    public:
        // do normal event
        auto DoEvent(UIControl* sender, const EventArg& e) noexcept->EventAccept override;
        // do mouse event
        //auto DoMouseEvent(const MouseEventArg& e) noexcept->EventAccept override;
        // update, postpone change some data
        void Update() noexcept override;
        // render this control only, [Global rendering and Incremental rendering]
        //void Render() const noexcept override;
        // recreate/init device(gpu) resource
        //auto Recreate() noexcept->Result override;
    protected:
        // add child
        void add_child(UIControl& child) noexcept override;
        // relayout
        void relayout() noexcept;
        // init tabbox
        void init_tabbox() noexcept;
    protected:
        // tabs
        UITabs*                 m_pTabs = nullptr;
        // tabpanels
        UITabPanels*            m_pTabPanels = nullptr;
        // current selected index
        uint32_t                m_index = 0;
    private:
    };
    // get meta info for UITabBox
    LUI_DECLARE_METAINFO(UITabBox);
}
