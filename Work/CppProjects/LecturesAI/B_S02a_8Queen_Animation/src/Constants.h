//
//  Created by Izudin Dzafic on 18/10/2025.
//  Copyright © 2025 IDz. All rights reserved.
//

#pragma once
#include <td/Types.h>

constexpr td::BYTE cMenuApp = 10;
constexpr td::BYTE cMenuAnimation = 30;
enum class Message : td::BYTE {Stop=0, PlayQueenPlaced, PlayPiecesCollected};
