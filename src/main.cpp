#include <ge.hpp>

#include <iostream>

class PrintDelta : public ge::ent::Component {
public:
    void update(ge::Ge& ge, ge::ent::Entity entity, double delta) override {
        std::cout << entity << ": " << delta << "\n";
    }
};

void spawn_print_delta(ge::Ge& ge){
    auto& ent = ge.ent();

    auto entity = ent.create_entity();
    ent.register_component<PrintDelta>();
    ent.spawn_component<PrintDelta>(entity, PrintDelta());
}

int main() {
    auto ge = ge::GeInit()
            .add_startup_system(spawn_print_delta)
            .init();
    ge.run();

    return 0;
}
