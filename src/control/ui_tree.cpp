﻿// LUI
#include <control/ui_tree.h>
#include <control/ui_treerow.h>
#include <control/ui_treecol.h>
#include <control/ui_treecols.h>
#include <control/ui_treeitem.h>
#include <control/ui_treecell.h>
#include <control/ui_treechildren.h>
#include <control/ui_ctrlmeta.h>

#include <core/ui_manager.h>
#include <input/ui_kminput.h>
#include <control/ui_image.h>
#include <debugger/ui_debug.h>
#include <constexpr/const_bkdr.h>

// PRIVATE
#include "../private/ui_private_control.h"
// C++
#include <algorithm>

// ACCESSIBLE
#ifdef LUI_ACCESSIBLE
#include <accessible/ui_accessible_callback.h>
#include <accessible/ui_accessible_event.h>
#include <accessible/ui_accessible_type.h>
#endif

// ui namespace
namespace LongUI {
    // UITree类 元信息
    LUI_CONTROL_META_INFO(UITree, "tree");
    // UITreeRow类 元信息
    LUI_CONTROL_META_INFO(UITreeRow, "treerow");
    // UITreeCol类 元信息
    LUI_CONTROL_META_INFO(UITreeCol, "treecol");
    // UITreeCols类 元信息
    LUI_CONTROL_META_INFO(UITreeCols, "treecols");
    // UITreeItem类 元信息
    LUI_CONTROL_META_INFO(UITreeItem, "treeitem");
    // UITreeCell类 元信息
    LUI_CONTROL_META_INFO(UITreeCell, "treecell");
    // UITreeChildren类 元信息
    LUI_CONTROL_META_INFO(UITreeChildren, "treechildren");
}


// ----------------------------------------------------------------------------
// --------------------             Tree              -------------------------
// ----------------------------------------------------------------------------



/// <summary>
/// Finalizes an instance of the <see cref="UITree"/> class.
/// </summary>
/// <returns></returns>
LongUI::UITree::~UITree() noexcept {
}


/// <summary>
/// Initializes a new instance of the <see cref="UITree"/> class.
/// </summary>
/// UITree(tree) 是树形控件的基础顶层控件, 所有的子控件需要包含在内部
/// <remarks>
/// </remarks>
/// <param name="parent">The parent.</param>
/// <param name="meta">The meta.</param>
LongUI::UITree::UITree(UIControl* parent, const MetaControl& meta) noexcept
    : Super(parent, meta) {
    // 树节点即为自己
    m_pTree = this;
    // 默认为列表框
    m_oStyle.appearance = Appearance_ListBox;
    // 默认样式
    m_oBox.border = { 1.f, 1.f, 1.f, 1.f };
    m_oBox.margin = { 4.f, 2.f, 4.f, 2.f };
}

/// <summary>
/// Updates this instance.
/// </summary>
/// <returns></returns>
void LongUI::UITree::Update() noexcept {
    // 要求重新布局
    if (this->is_need_relayout()) {
        // 不脏了
        m_state.dirty = false;
        // 重新布局
        this->relayout_base(m_pCols);
    }
    // 其他的交给父类处理
    Super::Update();
}


/// <summary>
/// Adds the child.
/// </summary>
/// <param name="child">The child.</param>
/// <returns></returns>
void LongUI::UITree::add_child(UIControl& child) noexcept {
    // TreeCols对象?
    if (uisafe_cast<UITreeCols>(&child)) {
        m_pCols = static_cast<UITreeCols*>(&child);
    }
    return Super::add_child(child);
}

/// <summary>
/// Adds the attribute.
/// </summary>
/// <param name="key">The key.</param>
/// <param name="value">The value.</param>
/// <returns></returns>
void LongUI::UITree::add_attribute(uint32_t key, U8View value) noexcept {
    constexpr auto BKDR_SELTYPE = 0xdee1c438_ui32;
    switch (key)
    {
    case BKDR_SELTYPE:
        // seltype      : 选择类型
        m_seltype = AttrParser::Seltype(value);
        return;
    default:
        // 其他交给父类处理
        return Super::add_attribute(key, value);
    }
}

/// <summary>
/// Does the event.
/// </summary>
/// <param name="sender">The sender.</param>
/// <param name="e">The e.</param>
/// <returns></returns>
auto LongUI::UITree::DoEvent(UIControl* sender,
    const EventArg& e) noexcept -> EventAccept {
    switch (e.nevent)
    {
    case NoticeEvent::Event_RefreshBoxMinSize:
        this->refresh_minsize(m_pCols);
        return Event_Accept;
    default:
        // 其他事件
        return Super::DoEvent(sender, e);
    }
}


/// <summary>
/// Selects the item.
/// </summary>
/// <param name="item">The item.</param>
/// <param name="exadd">if set to <c>true</c> [exadd].</param>
/// <returns></returns>
void LongUI::UITree::SelectItem(UITreeItem& item, bool exadd) noexcept {
    // 在表内额外添加就算了
    if (item.IsMarked() && exadd) return;
    // 记录上次操作对象
    m_pLastOp = &item;
    // 清空?
    if (!exadd || !this->IsMultiple()) this->ClearAllSelected();
    // 写入表内
    this->select_item(item);
}


/// <summary>
/// Clears the selected.
/// </summary>
/// <param name="item">The item.</param>
/// <returns></returns>
void LongUI::UITree::ClearSelected(UITreeItem& item) noexcept {

}

/// <summary>
/// Clears all selected.
/// </summary>
/// <returns></returns>
void LongUI::UITree::ClearAllSelected() noexcept {

}

/// <summary>
/// Selects the item.
/// </summary>
/// <param name="item">The item.</param>
/// <returns></returns>
void LongUI::UITree::select_item(UITreeItem& item) noexcept {

}

#ifdef LUI_ACCESSIBLE

/// <summary>
/// Accessibles the specified .
/// </summary>
/// <param name="args">The arguments.</param>
/// <returns></returns>
auto LongUI::UITree::accessible(const AccessibleEventArg& args) noexcept -> EventAccept {
    // 分类处理
    switch (args.event)
    {
        using get0_t = AccessibleGetPatternsArg;
        using get1_t = AccessibleGetCtrlTypeArg;
        using get2_t = AccessibleGetAccNameArg;
    case AccessibleEvent::Event_All_GetControlType:
        // 获取控件类型
        static_cast<const get1_t&>(args).type =
            AccessibleControlType::Type_Tree;
        return Event_Accept;
    case AccessibleEvent::Event_All_GetAccessibleName:
        // 获取Acc名称
        return UIControl::accessible(args);
    }
    return Super::accessible(args);
}

#endif

// ----------------------------------------------------------------------------
// --------------------           Tree Row            -------------------------
// ----------------------------------------------------------------------------

// ui 命名空间
namespace LongUI {
    // UITreeRow私有信息
    struct UITreeRow::Private : CUIObject {
        // 构造函数
        Private(UITreeRow& item) noexcept;
#ifndef NDEBUG
        // 调试占位
        void*               placeholder_debug1 = nullptr;
#endif
        // 开关控件
        UIImage             twisty;
        // 图像控件
        UIImage             image;
    };
    /// <summary>
    /// treerow privates data/method
    /// </summary>
    /// <param name="item">The item.</param>
    /// <returns></returns>
    UITreeRow::Private::Private(UITreeRow& item) noexcept
        : twisty(&item), image(&item) {
        //UIControlPrivate::SetFocusable(image, false);
        //UIControlPrivate::SetFocusable(label, false);
#ifndef NDEBUG
        twisty.name_dbg = "treerow::twisty";
        image.name_dbg = "treerow::image";
#endif
        UIControlPrivate::SetAppearance(twisty, Appearance_TreeTwisty);
    }
}

/// <summary>
/// Finalizes an instance of the <see cref="UITreeRow"/> class.
/// </summary>
/// <returns></returns>
LongUI::UITreeRow::~UITreeRow() noexcept {
    // 拥有额外的操作, 先标记析构
    m_state.destructing = true;
    if (m_private) delete m_private;
}

/// <summary>
/// Sets the has child.
/// </summary>
/// <param name="has">if set to <c>true</c> [has].</param>
/// <returns></returns>
void LongUI::UITreeRow::SetHasChild(bool has) noexcept {
    auto& sstate = UIControlPrivate::GetStyleState(m_private->twisty);
    sstate.indeterminate = has;
    m_bHasChild = has;
}


/// <summary>
/// Opens the node.
/// </summary>
/// <returns></returns>
bool LongUI::UITreeRow::OpenNode() noexcept {
    return m_bOpened ? false : (this->open_close(true), true);
}

/// <summary>
/// Closes the node.
/// </summary>
/// <returns></returns>
bool LongUI::UITreeRow::CloseNode() noexcept {
    return m_bOpened ? (this->open_close(false), true) : false ;
}

/// <summary>
/// Toggles the node.
/// </summary>
/// <returns></returns>
bool LongUI::UITreeRow::ToggleNode() noexcept {
    if (!m_bHasChild) return false;
    return m_bOpened ? CloseNode() : OpenNode();
}

/// <summary>
/// Sets the level offset.
/// </summary>
/// <param name="offset">The offset.</param>
/// <returns></returns>
void LongUI::UITreeRow::SetLevelOffset(float offset) noexcept {
    m_fLevelOffset = offset;
    //if (m_fLevelOffset != offset) {
    //    m_fLevelOffset = offset;
    //    this->NeedRelayout();
    //}
}

/// <summary>
/// Closes the node.
/// </summary>
/// <returns></returns>
void LongUI::UITreeRow::open_close(bool open) noexcept {
    m_bOpened = open;
    // 修改twisty状态
    auto& twisty = m_private->twisty;
    const auto statetp = StyleStateType::Type_Closed;
    twisty.StartAnimation({ statetp, !open });
    // 父节点必须是UITreeItem
    if (const auto item = longui_cast<UITreeItem*>(m_pParent)) {
        item->TreeChildrenOpenClose(open);
    }
}

/// <summary>
/// Initializes a new instance of the <see cref="UITreeRow"/> class.
/// </summary>
/// <remarks>
/// treerow 表示树形控件的一行, 父控件是treeitem, 子控件是treecell
/// 在XUL中主要作用就是用来展开收拢树形控件的一行
/// </remarks>
/// <param name="parent">The parent.</param>
/// <param name="meta">The meta.</param>
LongUI::UITreeRow::UITreeRow(UIControl* parent, const MetaControl& meta) noexcept
    : Super(parent, meta) {
    // 暂时用ListItem?
    m_oStyle.appearance = Appearance_ListItem;
    // TODO: 默认是关闭状态?

    // 私有实现
    m_private = new(std::nothrow) Private{ *this };
    // OOM处理
    this->ctor_failed_if(m_private);
    // 一开始假定没有数据
    if (m_private) this->SetHasChild(false);
}

/// <summary>
/// Updates this instance.
/// </summary>
/// <returns></returns>
void LongUI::UITreeRow::Update() noexcept {
    // 要求重新布局
    if (this->is_need_relayout()) {
        // 不脏了
        m_state.dirty = false;
        // 重新布局
        this->relayout();
    }
    // 其他的交给父类处理
    Super::Update();
}

/// <summary>
/// Does the event.
/// </summary>
/// <param name="sender">The sender.</param>
/// <param name="e">The e.</param>
/// <returns></returns>
auto LongUI::UITreeRow::DoEvent(UIControl* sender, const EventArg& e) noexcept->EventAccept {
    switch (e.nevent)
    {
    case NoticeEvent::Event_ImageChildClicked:
        // 点击了 twisty?
        if (sender == &m_private->twisty) {
            this->ToggleNode();
        }
        return Event_Accept;
    case NoticeEvent::Event_RefreshBoxMinSize:
        this->refresh_minsize();
        return Event_Accept;
    default:
        // 其他事件
        return Super::DoEvent(sender, e);
    }
}


/// <summary>
/// Does the mouse event.
/// </summary>
/// <param name="e">The e.</param>
/// <returns></returns>
auto LongUI::UITreeRow::DoMouseEvent(const MouseEventArg& e) noexcept -> EventAccept {
    switch (e.type)
    {
    case LongUI::MouseEvent::Event_LButtonDown:
        // 父节点必须是UITreeItem
        if (const auto item = longui_cast<UITreeItem*>(m_pParent)) {
            // 判断SHIFT
            if (CUIInputKM::GetKeyState(CUIInputKM::KB_SHIFT)) {
                item->SelectToThis();
            }
            else {
                // 判断CTRL键
                const auto ctrl = CUIInputKM::GetKeyState(CUIInputKM::KB_CONTROL);
                item->Select(ctrl);
            }
        }
        [[fallthrough]];
    default:
        return Super::DoMouseEvent(e);
    }
}

/// <summary>
/// Refreshes the minsize.
/// </summary>
/// <returns></returns>
void LongUI::UITreeRow::refresh_minsize() noexcept {
    float width = m_fLevelOffset;
    float height = 0.f;
    // 遍历
    for (auto& child : (*this)) {
        const auto ms = child.GetMinSize();
        width += ms.width;
        height = std::max(height, ms.height);
    }
    // 加上间距
    this->set_contect_minsize({ width, height });
}

/// <summary>
/// Relayouts this instance.
/// </summary>
/// <returns></returns>
void LongUI::UITreeRow::relayout() noexcept {
    // 没有cols时调用
    const auto when_no_cols = [this]() noexcept {
        const auto ctsize = this->GetBox().GetContentSize();
        float xoffset = m_fLevelOffset;
        // 遍历有效子节点
        for (auto& child : (*this)) {
            // TODO: maxsize足够小应该居中放置
            child.SetPos({ xoffset, 0.f });
            const auto min = child.GetMinSize();
            this->resize_child(child, { min.width, ctsize.height });
            xoffset += min.width;
        }
    };
    // 存在cols时调用
    const auto when_cols = [this, when_no_cols](UITreeCols* cols) noexcept {
        const auto ctsize = this->GetBox().GetContentSize();
        float xoffset = m_fLevelOffset;
        const auto set_child = [&xoffset, ctsize](UIControl& child) noexcept {
            child.SetPos({ xoffset, 0.f });
            const auto min = child.GetMinSize();
            UITreeRow::resize_child(child, { min.width, ctsize.height });
            xoffset += min.width;
        };
        // twisty
        set_child(m_private->twisty);
        // image
        set_child(m_private->image);
        
        // (image, end)
        auto itr = ++Iterator<UIControl>{ &m_private->image };


        for (auto& col : (*cols)) {
            // 必须是col
            if (!uisafe_cast<UITreeCol>(&col)) continue;
            // 提前出局
            if (itr == this->end()) break;
            const auto pos = col.GetPos();
            const auto size = col.GetSize();
            auto& child = *itr;
            child.SetPos({ pos.x + xoffset, pos.y });
            const auto width = size.width - xoffset;
            this->resize_child(child, { width, size.height });
            ++itr;
        }

    };
    // 父节点是treeitem
    if (const auto treeitem = longui_cast<UITreeItem*>(m_pParent)) {
        const auto tree = treeitem->GetTreeNode();
        assert(tree && "bad tree");
        // 获取树列
        if (const auto cols = tree->GetCols()) 
            return when_cols(cols);
    }
    // 没有cols
    when_no_cols();
}


// ----------------------------------------------------------------------------
// --------------------           Tree Item           -------------------------
// ----------------------------------------------------------------------------




/// <summary>
/// Finalizes an instance of the <see cref="UITreeItem"/> class.
/// </summary>
/// <returns></returns>
LongUI::UITreeItem::~UITreeItem() noexcept {

}


/// <summary>
/// Initializes a new instance of the <see cref="UITreeItem" /> class.
/// </summary>
/// <remarks>
/// UITreeItem(treeitem) 应该被直接包含在treechildren下面, 以及包含treerow控件
/// treeitem 可以被用户点击以选择树形控件的一行
/// </remarks>
/// <param name="parent">The parent.</param>
/// <param name="meta">The meta.</param>
LongUI::UITreeItem::UITreeItem(UIControl* parent, const MetaControl& meta) noexcept
    : Super(parent, meta) {
#ifdef LUI_ACCESSIBLE
    // 默认逻辑对象为空
    m_pAccCtrl = nullptr;
#endif
}


/// <summary>
/// Adds the child.
/// </summary>
/// <param name="child">The child.</param>
/// <returns></returns>
void LongUI::UITreeItem::add_child(UIControl& child) noexcept {
    // 是TreeChildren?
    if (uisafe_cast<UITreeChildren>(&child)) {
        m_pChildren = static_cast<UITreeChildren*>(&child);
#ifdef LUI_ACCESSIBLE
        // 有children就使用
        m_pAccCtrl = m_pChildren;
#endif
    }
    // 是TreeRow?
    else if (uisafe_cast<UITreeRow>(&child)) {
        m_pRow = static_cast<UITreeRow*>(&child);
    }
#ifndef NDEBUG
    // 不允许其他类型的子节点
    else {
        if (uisafe_cast<UITreeCols>(&child)) {

        }
        else {
            assert(!"cannot add other type to treeitem");
        }
    }
#endif
    return Super::add_child(child);
}



/// <summary>
/// Updates this instance.
/// </summary>
/// <returns></returns>
void LongUI::UITreeItem::Update() noexcept {
    // 要求重新布局
    if (this->is_need_relayout()) {
        // 不脏了
        m_state.dirty = false;
        // 重新布局
        this->relayout_base(m_pRow);
        // 偏移
        if (m_pRow) {
            //const auto mh = m_pRow->GetMinSize().height * 0.5f;
            m_pRow->SetLevelOffset(m_fLevelOffset);
        }
    }
    // 其他的交给父类处理
    Super::Update();
}


/// <summary>
/// Trees the children changed.
/// </summary>
/// <param name="has_child">if set to <c>true</c> [has child].</param>
/// <returns></returns>
void LongUI::UITreeItem::TreeChildrenChanged(bool has_child) noexcept {
    if (m_pRow) m_pRow->SetHasChild(has_child);
}


/// <summary>
/// Selects to this.
/// </summary>
/// <returns></returns>
void LongUI::UITreeItem::SelectToThis() noexcept {

}

/// <summary>
/// Selects this instance.
/// </summary>
/// <returns></returns>
void LongUI::UITreeItem::Select(bool exsel) noexcept {
    int bk = 9;
}


/// <summary>
/// Trees the children close.
/// </summary>
/// <param name="open">if set to <c>true</c> [open].</param>
/// <returns></returns>
void LongUI::UITreeItem::TreeChildrenOpenClose(bool open) noexcept {
    assert(m_pChildren && "bad apple??");
    if (m_pChildren) {
        m_pChildren->SetVisible(open);
    }
}

/// <summary>
/// Does the event.
/// </summary>
/// <param name="sender">The sender.</param>
/// <param name="e">The e.</param>
/// <returns></returns>
auto LongUI::UITreeItem::DoEvent(UIControl* sender,
    const EventArg& e) noexcept -> EventAccept {
    switch (e.nevent)
    {
    case NoticeEvent::Event_RefreshBoxMinSize:
        this->refresh_minsize(m_pRow);
        return Event_Accept;
    default:
        // 其他事件
        return Super::DoEvent(sender, e);
    }
}

PCN_NOINLINE
/// <summary>
/// Relayouts the tree.
/// </summary>
/// <param name="head">The head.</param>
/// <returns></returns>
void LongUI::UITreeItem::relayout_base(UIControl* head) noexcept {
    // XXX: 获取左上角位置
    const auto lt = this->GetBox().GetContentPos();
    float xofffset = m_fLevelOffset;
    float yoffset = lt.y;
    const auto ctwidth= this->GetBox().GetContentSize().width;
    // 拥有头对象?
    if (head) {
        // 位置
        head->SetPos(lt);
        // 层偏移量
        //m_pRow->SetLevelOffset(m_fLevelOffset);
        // 大小
        const auto height = head->GetMinSize().height;
        this->resize_child(*head, { ctwidth, height });
        // 调整children数据
        yoffset += height;
        // 第一层实际是0的偏移
        if (head == m_pRow) xofffset += height * 0.5f;
    }
    // 拥有子节点对象?
    if (m_pChildren && m_pChildren->IsVisible()) {
        // 调整层级
        m_pChildren->SetLevelOffset(xofffset);
        // 位置
        m_pChildren->SetPos({ lt.x, yoffset });
        // 大小
        const auto mh = m_pChildren->GetMinSize().height;
        this->resize_child(*m_pChildren, { ctwidth, mh });
    }
}

PCN_NOINLINE
/// <summary>
/// Refreshes the minsize.
/// </summary>
/// <returns></returns>
void LongUI::UITreeItem::refresh_minsize(UIControl* head) noexcept {
    float height = 0.f;
    // 加上内容
    if (head)
        height += head->GetMinSize().height;
    if (m_pChildren && m_pChildren->IsVisible())
        height += m_pChildren->GetMinSize().height;
    // 加上间距
    this->set_contect_minsize({ 0, height });
}

#ifdef LUI_ACCESSIBLE

/// <summary>
/// Accessibles the specified .
/// </summary>
/// <param name="args">The arguments.</param>
/// <returns></returns>
auto LongUI::UITreeItem::accessible(const AccessibleEventArg& args) noexcept -> EventAccept {
    // 获取ACC名称
    const auto get_acc_name = [this](CUIString& text) noexcept {
        if (m_pRow) {
            for (auto& child : (*m_pRow)) {
                if (const auto cell = uisafe_cast<UITreeCell>(&child)) {
                    if (!text.empty()) text += L' ';
                    text += cell->GetTextString();
                }
            }
        }
    };
    // 分类处理
    switch (args.event)
    {
        using get0_t = AccessibleGetPatternsArg;
        using get1_t = AccessibleGetCtrlTypeArg;
        using get2_t = AccessibleGetAccNameArg;
        //case AccessibleEvent::Event_GetPatterns:
        //    // + 继承基类行为模型
        //    Super::accessible(args);
        //    static_cast<const get0_t&>(args).patterns |=
        //        // + 可调用的行为模型
        //        Pattern_Invoke
        //        // + 读写值的行为模型
        //        | Pattern_Value
        //        ;
        //    return Event_Accept;
    case AccessibleEvent::Event_All_GetControlType:
        // 获取控件类型
        static_cast<const get1_t&>(args).type =
            AccessibleControlType::Type_TreeItem;
        return Event_Accept;
    case AccessibleEvent::Event_All_GetAccessibleName:
        // 获取Acc名称
        get_acc_name(*static_cast<const get2_t&>(args).name);
        return Event_Accept;
    }
    return Super::accessible(args);
}
#endif

// ----------------------------------------------------------------------------
// --------------------        Tree Children          -------------------------
// ----------------------------------------------------------------------------



/// <summary>
/// Finalizes an instance of the <see cref="UITreeChildren"/> class.
/// </summary>
/// <returns></returns>
LongUI::UITreeChildren::~UITreeChildren() noexcept {

}


/// <summary>
/// Sets the level offset.
/// </summary>
/// <param name="offset">The offset.</param>
void LongUI::UITreeChildren::SetLevelOffset(float offset) {
    for (auto& child : (*this)) {
        // 子节点必须是UITreeItem
        if (const auto ptr = longui_cast<UITreeItem*>(&child)) {
            ptr->TreeLevelOffset(offset);
        }
    }
}

/// <summary>
/// Initializes a new instance of the <see cref="UITreeChildren"/> class.
/// </summary>
/// UITreeChildren(treechildren) 是为了嵌套树的一系列子控件的一个容器
/// 其父控件必须是treeitem(或是tree), 其子控件们也必须是treeitem
/// <param name="parent">The parent.</param>
/// <param name="meta">The meta.</param>
LongUI::UITreeChildren::UITreeChildren(UIControl* parent, const MetaControl& meta) noexcept
    : Super(parent, meta) {
    // 垂直布局
    this->SetOrient(Orient_Vertical);
}



/// <summary>
/// Updates this instance.
/// </summary>
/// <returns></returns>
void LongUI::UITreeChildren::Update() noexcept {
    // 父节点修改/子节点添加删除
    if (m_state.parent_changed || m_state.child_i_changed) {
        // 父节点必须是UITreeItem
        if (const auto item = longui_cast<UITreeItem*>(m_pParent)) {
            // 子节点也必须是UITreeItem
            for (auto& child : (*this)) {
                const auto ptr = longui_cast<UITreeItem*>(&child);
                ptr->AsSameTreeTo(*item);
            }
        }
    }
    // 子节点添加删除
    if (m_state.child_i_changed) {
        const auto has = !!this->GetCount();
        if (has != m_bHasChild) {
            m_bHasChild = has;
            // 通知父控件(父节点必须是UITreeItem)
            if (const auto item = longui_cast<UITreeItem*>(m_pParent)) {
                item->TreeChildrenChanged(has);
            }
        }
    }
    return Super::Update();
}


#ifndef NDEBUG
/// <summary>
/// Adds the child.
/// </summary>
/// <param name="child">The child.</param>
/// <returns></returns>
void LongUI::UITreeChildren::add_child(UIControl& child) noexcept {
    // TreeCols对象?
    if (uisafe_cast<UITreeItem>(&child)) {
    }
    else {
        assert(!"child to treechildren must be treeitem");
    }
    return Super::add_child(child);
}
#endif

// ----------------------------------------------------------------------------
// --------------------           Tree Col            -------------------------
// ----------------------------------------------------------------------------


/// <summary>
/// Finalizes an instance of the <see cref="UITreeCol"/> class.
/// </summary>
/// <returns></returns>
LongUI::UITreeCol::~UITreeCol() noexcept {

}


/// <summary>
/// Initializes a new instance of the <see cref="UITreeCol"/> class.
/// </summary>
/// <remarks>
/// UITreeCol(treecol) 其目的是为了方便设置列属性, 例如flex
/// 本身父控件必须是treecols
/// </remarks>
/// <param name="parent">The parent.</param>
/// <param name="meta">The meta.</param>
LongUI::UITreeCol::UITreeCol(UIControl* parent, const MetaControl& meta) noexcept
    : Super(parent, meta) {
}


// ----------------------------------------------------------------------------
// --------------------           Tree Cols           -------------------------
// ----------------------------------------------------------------------------


/// <summary>
/// Finalizes an instance of the <see cref="UITreeCols"/> class.
/// </summary>
/// <returns></returns>
LongUI::UITreeCols::~UITreeCols() noexcept {
}


/// <summary>
/// Initializes a new instance of the <see cref="UITreeCols"/> class.
/// </summary>
/// <remarks>
/// UITreeCols(treecols) 是treecol的父级控件(容器), 本身父控件必须是tree
/// 其目的是为了方便设置辣么多列属性, 例如flex
/// </remarks>
/// <param name="parent">The parent.</param>
/// <param name="meta">The meta.</param>
LongUI::UITreeCols::UITreeCols(UIControl* parent, const MetaControl& meta) noexcept
    : Super(parent, meta) {
}

/// <summary>
/// Does the event.
/// </summary>
/// <param name="e">The e.</param>
/// <returns></returns>
auto LongUI::UITreeCols::DoEvent(UIControl* sender, const EventArg& e) noexcept->EventAccept {
    switch (e.nevent)
    {
    case NoticeEvent::Event_Splitter:
        assert(m_pParent && "no parent");
        this->NeedRelayout();
        // 父节点是tree
        if (auto tc = longui_cast<UITree*>(m_pParent)->GetTreeChildren())
            UITreeCols::do_update_for_item(*tc);
        m_pParent->Invalidate();
        [[fallthrough]];
    default:
        return Super::DoEvent(sender, e);
    }
}

/// <summary>
/// Does the update for item.
/// </summary>
/// <param name="ctrl">The control.</param>
/// <returns></returns>
void LongUI::UITreeCols::do_update_for_item(UIControl& ctrl) noexcept {
    for (auto& child : ctrl) {
        if (uisafe_cast<UITreeItem>(&child)) {
            if (auto col = static_cast<UITreeItem&>(child).GetCol()) 
                col->NeedRelayout();
        }
        else {
            UITreeCols::do_update_for_item(child);
        }
    }
}

/// <summary>
/// Updates this instance.
/// </summary>
/// <returns></returns>
//void LongUI::UITreeCols::Update() noexcept {
//    return Super::Update();
//}

// ----------------------------------------------------------------------------
// --------------------           Tree Cell           -------------------------
// ----------------------------------------------------------------------------


/// <summary>
/// Finalizes an instance of the <see cref="UITreeCell"/> class.
/// </summary>
/// <returns></returns>
LongUI::UITreeCell::~UITreeCell() noexcept {
}


/// <summary>
/// Initializes a new instance of the <see cref="UITreeCell"/> class.
/// </summary>
/// <param name="parent">The parent.</param>
/// <param name="meta">The meta.</param>
LongUI::UITreeCell::UITreeCell(UIControl* parent, const MetaControl& meta) noexcept
    : Super(parent, meta) {
}

/// <summary>
/// Adds the attribute.
/// </summary>
/// <param name="key">The key.</param>
/// <param name="value">The value.</param>
/// <returns></returns>
void LongUI::UITreeCell::add_attribute(uint32_t key, U8View value) noexcept {
    constexpr auto BKDR_LABEL       = 0x74e22f74_ui32;
    constexpr auto BKDR_VALUE       = 0x246df521_ui32;
    switch (key)
    {
    case BKDR_LABEL:
        // label: 重定向至value
        key = BKDR_VALUE;
        [[fallthrough]];
    default:
        // 其他情况
        return Super::add_attribute(key, value);
    }
}

