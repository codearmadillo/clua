function createMenuState()
    local state = clua.create.state()

    function state.start()
        print("ola")
    end
    state.start();

    return state
end