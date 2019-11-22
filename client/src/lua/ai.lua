lua_keys = {}
lua_data = {}
lua_data_actions = { ["empty"] = "Y",
                     ["downLeft"] = "L",
                     ["downRight"] = "R",
                     ["upRight"] = "R",
                     ["upLeft"] = "L",
                     ["horizontal"] = "A",
                     ["vertical"] = "A",
                     ["finishH"] = "A",
                     ["finishV"] = "Y",
                     ["public1Up"] = "Y",
                     ["public1Down"] = "Y",
                     ["public1Left"] = "L",
                     ["public1Right"] = "R" };

function getNextMove(positionX, positionY)
    --[=====[
    print(positionX, positionY, track_matrix[positionX][positionY])
    --]=====]

    print(positionX, positionY, track_matrix[positionX][positionY],
            lua_data_actions[track_matrix[positionX][positionY]])

    return lua_data_actions[track_matrix[positionX][positionY]];
end
