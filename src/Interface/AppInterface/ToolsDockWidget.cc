/*
 For more information, please see: http://software.sci.utah.edu
 
 The MIT License
 
 Copyright (c) 2009 Scientific Computing and Imaging Institute,
 University of Utah.
 
 
 Permission is hereby granted, free of charge, to any person obtaining a
 copy of this software and associated documentation files (the "Software"),
 to deal in the Software without restriction, including without limitation
 the rights to use, copy, modify, merge, publish, distribute, sublicense,
 and/or sell copies of the Software, and to permit persons to whom the
 Software is furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included
 in all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 DEALINGS IN THE SOFTWARE.
*/
 
#include <sstream>
#include <iostream>

#include <Utils/Core/Log.h>
#include <boost/lexical_cast.hpp>

// Application includes
#include <Application/Tool/ToolManager.h>
#include <Application/Tool/ToolFactory.h>
#include <Application/Interface/Interface.h>

// Interface includes
#include <Interface/AppInterface/ToolsDockWidget.h>

#include "ui_ToolsDockWidget.h"

namespace Seg3D  {

class ToolsDockWidgetPrivate {
  public:
    Ui::ToolsDockWidget ui_;
  
};


ToolsDockWidget::ToolsDockWidget(QWidget *parent) :
    QDockWidget(parent),
    private_(new ToolsDockWidgetPrivate)
{

  toolbox_ = new ToolBoxWidget(this);
  make_dock_widget();


  // Ensure that the application does not change any of the tools while
  // the user interface is being built
  ToolManager::Instance()->lock_tool_list();
  
  // Forward the open tool:
  // This function binds a set of functions together: (1) it post a new function
  // message, it will check whether the pointer still exists to the current 
  // QObject and then execute the function on the interface thread.

  QPointer<ToolsDockWidget> qpointer(this);

  open_tool_connection_ =
    ToolManager::Instance()->connect_open_tool(
        boost::bind(&ToolsDockWidget::post_open_tool,qpointer,_1));

  close_tool_connection_ =
    ToolManager::Instance()->connect_close_tool(
        boost::bind(&ToolsDockWidget::post_close_tool,qpointer,_1));

  activate_tool_connection_ =
    ToolManager::Instance()->connect_activate_tool(
        boost::bind(&ToolsDockWidget::post_activate_tool,qpointer,_1));

  ToolManager::tool_list_type tool_list = ToolManager::Instance()->tool_list();
  ToolManager::tool_list_type::iterator it = tool_list.begin();
  ToolManager::tool_list_type::iterator it_end = tool_list.end();

  while (it != it_end)
  {
    // Call open_tool for each tool in the list
    open_tool((*it).second);
    ++it;
  }
  
  std::string active_toolid = ToolManager::Instance()->active_toolid();
  ToolManager::tool_list_type::iterator active_it = tool_list.find(active_toolid);

  // Set the active tool
  if (active_it != it_end)
  {
    activate_tool((*active_it).second);
  } 

  // Connect the signal that the active tool was changed to dispatching
  // an action in the application.
  connect(toolbox_,SIGNAL(currentChanged(int)),this,SLOT(tool_changed(int)));
  
  // Now the tool list is up to date we can release the lock on the ToolManager
  ToolManager::Instance()->unlock_tool_list();
  
}
  
  
// Build the GUI
void ToolsDockWidget::make_dock_widget()
{
  QWidget *dockWidgetContents;
  QVBoxLayout *verticalLayout;
  QVBoxLayout *verticalLayout_2;
  QScrollArea *scrollArea;
  QWidget *scrollAreaWidgetContents;
  QVBoxLayout *verticalLayout_3;
  QSpacerItem *verticalSpacer;
  
  
  this->resize(243, 640);
  QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
  sizePolicy.setHorizontalStretch(0);
  sizePolicy.setVerticalStretch(0);
  sizePolicy.setHeightForWidth(this->sizePolicy().hasHeightForWidth());
  this->setSizePolicy(sizePolicy);
  this->setMinimumSize(QSize(243, 184));
  this->setMaximumSize(QSize(243, 524287));
  this->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
  this->setWindowTitle("Tools/Plugins");

  dockWidgetContents = new QWidget();
  dockWidgetContents->setObjectName(QString::fromUtf8("dockWidgetContents"));
  verticalLayout_2 = new QVBoxLayout(dockWidgetContents);
  verticalLayout_2->setSpacing(0);
  verticalLayout_2->setContentsMargins(0, 0, 0, 0);
  verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
  scrollArea = new QScrollArea(dockWidgetContents);
  scrollArea->setObjectName(QString::fromUtf8("scrollArea"));
  QSizePolicy sizePolicy1(QSizePolicy::Fixed, QSizePolicy::MinimumExpanding);
  sizePolicy1.setHorizontalStretch(0);
  sizePolicy1.setVerticalStretch(0);
  sizePolicy1.setHeightForWidth(scrollArea->sizePolicy().hasHeightForWidth());
  scrollArea->setSizePolicy(sizePolicy1);
  scrollArea->setMinimumSize(QSize(243, 29));
  scrollArea->setStyleSheet(QString::fromUtf8(""));
  scrollArea->setWidgetResizable(true);
  scrollAreaWidgetContents = new QWidget();
  scrollAreaWidgetContents->setObjectName(QString::fromUtf8("scrollAreaWidgetContents"));
  scrollAreaWidgetContents->setGeometry(QRect(0, 0, 239, 610));
  QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::MinimumExpanding);
  sizePolicy2.setHorizontalStretch(0);
  sizePolicy2.setVerticalStretch(0);
  sizePolicy2.setHeightForWidth(scrollAreaWidgetContents->sizePolicy().hasHeightForWidth());
  scrollAreaWidgetContents->setSizePolicy(sizePolicy2);
  verticalLayout_3 = new QVBoxLayout(scrollAreaWidgetContents);
  verticalLayout_3->setSpacing(0);
  verticalLayout_3->setContentsMargins(0, 0, 0, 0);
  verticalLayout_3->setObjectName(QString::fromUtf8("verticalLayout_3"));
  verticalLayout = new QVBoxLayout();
  verticalLayout->setSpacing(0);
  verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
 
  
  verticalLayout->addWidget(toolbox_, Qt::AlignTop|Qt::AlignHCenter);
  
  verticalSpacer = new QSpacerItem(20, 406, QSizePolicy::Minimum, QSizePolicy::Preferred);
  
  verticalLayout->addItem(verticalSpacer);
  
  
  verticalLayout_3->addLayout(verticalLayout);
  
  scrollArea->setWidget(scrollAreaWidgetContents);
  
  verticalLayout_2->addWidget(scrollArea, Qt::AlignTop|Qt::AlignHCenter);
  
   
  this->setWidget(dockWidgetContents);
  
}

  


ToolsDockWidget::~ToolsDockWidget()
{
  open_tool_connection_.disconnect();
  close_tool_connection_.disconnect();
  activate_tool_connection_.disconnect();
}

void
ToolsDockWidget::open_tool(ToolHandle tool)
{
  // Step (1) : find the widget class in the ToolFactory
  ToolInterface *interface;
  ToolFactory::Instance()->create_toolinterface(tool->type(),interface);
  
  ToolWidget *widget = dynamic_cast<ToolWidget*>(interface);
  if (widget == 0)
  {
    SCI_THROW_LOGICERROR("A ToolInterface cannot be up casted to a ToolWidget pointer");
  }
  // Step (2) : instantiate the widget
  widget->create_widget(this,tool);
  //widget->resize(215, 29);
  std::string h = boost::lexical_cast<std::string>(widget->height());
  SCI_LOG_MESSAGE("The the height of the widget is: "+ h);
   
  toolbox_->add_tool(widget,QString::fromStdString(tool->menu_name()
                                                  +" "+Utils::to_string(tool->toolid_number())));
  widget_list_[tool->toolid()] = widget;
}

void
ToolsDockWidget::close_tool(ToolHandle tool)
{
  // Find the widget
  widget_list_type::iterator it = widget_list_.find(tool->toolid());
  if (it == widget_list_.end()) 
  {
    SCI_LOG_ERROR(std::string("widget with toolid '")+tool->toolid()+"' does not exist");
    return;
  } 
  ToolWidget *widget = (*it).second;
  
  // Remove this widget from the widget
  //toolbox_->removeItem(toolbox_->indexOf(widget));
  toolbox_->remove_tool(toolbox_->index_of(widget));
  // Remove the pointer from widget 
  widget_list_.erase(tool->toolid());
  
  // Schedule object to be destroyed by Qt
  widget->deleteLater();
}

void
ToolsDockWidget::activate_tool(ToolHandle tool)
{
  // Ensure that this call is migrated to the interface thread
  if (!Interface::IsInterfaceThread())
  {
    Interface::PostEvent(boost::bind(&ToolsDockWidget::activate_tool,this,tool));
    return;
  }

  // Find the widget
  widget_list_type::iterator it = widget_list_.find(tool->toolid());
  if (it == widget_list_.end()) 
  {
    SCI_LOG_ERROR(std::string("widget with toolid '")+tool->toolid()+"' does not exist");
    return;
  } 
  ToolWidget *widget = (*it).second;
  
  if (widget != toolbox_->get_active_tool())
  {
    toolbox_->set_active_tool(widget);  
  }
}

void
ToolsDockWidget::tool_changed(int index)
{
  if (index > 0)
  {
    ToolWidget *widget = static_cast<ToolWidget*>(toolbox_->get_tool_at(index));
    //ToolWidget *widget = static_cast<ToolWidget*>(toolbox_->widget(index));
    ToolManager::Instance()->dispatch_activatetool(widget->toolid());
  }
}


void
ToolsDockWidget::post_open_tool(QPointer<ToolsDockWidget> widget,ToolHandle tool)
{
  SCI_LOG_DEBUG("Post Open Tool");
  if (!(Interface::IsInterfaceThread()))
  {
    Interface::Instance()->post_event(boost::bind(&ToolsDockWidget::post_open_tool,widget,tool));
    return;
  }
  
  if (widget) widget->open_tool(tool);
}

void
ToolsDockWidget::post_close_tool(QPointer<ToolsDockWidget> widget,ToolHandle tool)
{
  if (!(Interface::IsInterfaceThread()))
  {
    SCI_LOG_MESSAGE("Posting close tool");
    Interface::Instance()->post_event(boost::bind(&ToolsDockWidget::post_close_tool,widget,tool));
    return;
  }
  
  if (widget) widget->close_tool(tool);
}

void
ToolsDockWidget::post_activate_tool(QPointer<ToolsDockWidget> widget,ToolHandle tool)
{
  if (!(Interface::IsInterfaceThread()))
  {
    Interface::Instance()->post_event(boost::bind(&ToolsDockWidget::post_activate_tool,widget,tool));
    return;
  }
  
  if (widget) widget->activate_tool(tool);
}




} // end namespace