// ################################################################################################################
// # native Interface Design (natID)
// # Licensed under the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # You may use this code under the terms of the Creative Commons Attribution-NoDerivatives (CC BY-ND), version 4.
// # 
// # Contact: idzafic at etf.unsa.ba  or idzafic at gmail.com
// ################################################################################################################

/** @file IMessageConsumer.h
    @brief Interface for objects that can consume GUI framework messages and special key events. */
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

    /// @brief Abstract interface for UI components that receive and process framework messages.
	class IMessageConsumer
	{
	public:
        /// @brief Indicates the readiness state of the consumer for accepting messages.
        enum class Status: unsigned char
        {
            NotReady, ///< Consumer is not ready to receive messages.
            Ready,    ///< Consumer is ready to receive messages.
            Blocked,  ///< Consumer is temporarily blocked from receiving messages.
            NA        ///< Status is not applicable.
        };

		//virtual bool onMessage(gui::Message& msg) = 0;

        /// @brief Called when a special character key is pressed.
        /// @param pSender Pointer to the object that originated the key event.
        /// @param key The special character that was pressed.
        /// @return True if the event was handled, false to allow further processing.
		virtual bool onSpecialChar(gui::Object* pSender, char key) = 0;

        /// @brief Internal framework method for processing a generic GUI message.
        /// @param msg Reference to the message to process.
        /// @return True if the message was handled.
		virtual bool _processFrameworkMessage(gui::Message& msg) = 0;

        /// @brief Checks whether the consumer is currently ready to accept a message.
        /// @return True if the consumer is ready to receive messages.
		virtual bool isReadyForMessage() const = 0;
	};
}
