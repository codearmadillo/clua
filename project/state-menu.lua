function createMenuState()
    local state = clua.state.create()

    print(state.start, state.update, state.draw, state.destroy, state.setAsDefault, state.setAsActive)

    -- Define state-specific functionality
    state.start = function()
        print("state starting")
    end
    state.update = function(deltaTime)
        print("state updating")
    end
    state.draw = function()
        print("state drawing")
    end
    state.destroy = function()
        print("state ending")
    end
    -- This will have to be defined from Clua
    state.setAsDefault = function()
        print("set as default")
    end
    -- Return state
    return state
end