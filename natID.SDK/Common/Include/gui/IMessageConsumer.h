// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

//
//  Created by Izudin Dzafic on 28/07/2020.
//  Copyright © 2020 IDz. All rights reserved.
//

#pragma once
#include <td/Variant.h>
#include <gui/Message.h>

namespace gui
{
    class Object;
	class IMessageConsumer
	{
	public:
        enum class Status: unsigned char {NotReady, Ready, Blocked, NA};
        
		//virtual bool onMessage(gui::Message& msg) = 0;
		virtual bool onSpecialChar(gui::Object* pSender, char key) = 0;
		virtual bool _processFrameworkMessage(gui::Message& msg) = 0;
		virtual bool isReadyForMessage() const = 0;
	};
}
