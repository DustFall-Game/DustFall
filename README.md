# DustFall

## 1. Project Structure

Все файлы проекта должны быть организованы по единой иерархии:

```
Source/
└── DustFall/
    ├── Core/                 // Основные классы, типы, утилиты, GameMode, GameInstance
    │   ├── Player/           // Игровые персонажи
    │   ├── Inventory/        // Предметы, оружие, инвентарь
    │   └── Interfaces/       // Интерфейсы
    ├── UI/                   // Виджеты и HUD
    ├── AI/                   // Поведение ИИ, BT Tasks, Services, Decorators
    ├── World/                // Активные элементы карты (двери, триггеры и т.п.)
    └── Systems/              // Глобальные подсистемы (сохранения, сессии, настройки)
```

**Рекомендация:**  
Каждый класс должен находиться в отдельной папке, содержащей только его файлы.
Пример структуры:
```
Source/
└── Player/
    └── Character/
        ├── PlayerCharacter.h
        └── PlayerCharacter.cpp
```

**Каждый класс → отдельная подпапка!!!**

**Избегай общих папок с большим количеством файлов. Исключения:**
```
Interfaces/ — можно хранить все файлы интерфейсов(.cpp не обязателен, можно удалять)
UI/ — можно хранить несколько UI-классов.

Так же если у вас много однотипных файлов который не несут логики то их можно выносить в такую папку.
```

---

## 2. File Naming Rules

###  Общие правила
- Все файлы в `Content/` должны иметь логичный префикс, отражающий тип ассета.

| Тип ассета          | Префикс | Пример             | Комментарий                    |
| ------------------- | ------- | ------------------ | ------------------------------ |
| `Static Mesh`         | **SM_**     | SM_Chair           | Все статик меши                |
| `Skeletal Mesh`       | **SK_**     | SK_Player          | Анимационные меши              |
| `Physics Asset`       | **PA_**     | PA_Rabbit          | Физический ассет             |
| `Material`            | **M_**      | M_Wood             | Материалы                      |
| `Texture`             | **T_**      | T_WoodDiffuse      | Текстуры                       |
| `Sound`               | **S_**      | S_Footstep         | Звуковые файлы                 |
| `Sound Cue`           | **SC_**     | SC_Explosion       | Кастомные Cue                  |
| `Animation`           | **A_**      | A_Run              | Анимации для Skeletal Mesh     |
| `Animation Blueprint` | **ABP_**    | ABP_Player         | Blueprint для анимаций         |
| `Particle System`     | **P_**      | P_Fire             | Niagara / Particle             |
| `Blueprint Class`     | **BP_**     | BP_PlayerCharacter | Игровые Actor Blueprints       |
| `Widget Blueprint`    | **WBP_**    | WBP_MainMenu       | UI виджеты                     |
| `Data Table`          | **DT_**     | DT_Items           | Таблицы данных                 |
| `Enumeration`    | **Enum_**    | Enum_FireType       | Енумераторы                     |
| `Strcuture`          | **Struct_Weapon**     | Struct_           | Структура данных                 |
| `Input Action`    | **IA_Move**    | IA_       | Кнопка в система Enhanced Input                     |
| `Input Mapping Context`          | **IMC_**     | IMC_Default           | Таблицы Маппингов для управленния и назначенние кнопок                 |
| `Sound Attenuation`    | **SA_**    | SA_Room       | Настройки отглоса звука                     |
| `Sound Class`    | **SC_**    | SC_SFX       | Саунд настройки глобальные                     |
| `Sound Class Mix`          | **SCM_**     | SCM_Master           | Миксер глобальных настроек саунда                |
| `Behavior Tree`          | **BT_**     | BT_Animal           | Дерево поведения AI                |
| `Behavior Tree Task`          | **BTTask_**     | BTTask_RandomPatrol           | Задачи в дереве поведения                |
| `Blackboard`          | **BB_**     | BB_Animal           | Среда перменных для AI               |
| `Data Asset`          | **DA_**     | DA_House           | Ассет данных                |
---

## 3. Folder Naming Rules

- Папки именуются (`Components`, `UI`, `Core`, `Systems` и тд.).
- Названия папок должны отражать функциональность, а не технический тип.  
  ❌ `Actors/` → ✅ `Characters/` `Weapons/`.

---

## 4. Class Naming Conventions

### Общие правила
- Используй **описательные имена**.
- Класс должен содержать **единственную ответственность**.

```cpp
// Хорошо
class APlayerCharacter;
class UHealthComponent;
class UInventorySystem;

// Плохо
class AChar;
class UMyComp;
class UObjectHandler;
```

---

## 5. Variable Naming Conventions

| Категория                           | Пример                                      | Правило / описание                                                                                                                                                                                                                                         |
|-------------------------------------|---------------------------------------------| ---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| Обычные переменные                  | `CurrentHealth`, `MaxHealth`                | Имена должны быть понятными и описывать, что хранит переменная и всегда должны быть перменные с большой буквы кроме булевых. Например: `PlayerSpeed`, `DamageAmount`.                                                                                                                          |
| Булевые переменные                  | `bIsDead`, `bCanJump`                       | Начинай с **`b` + Название**, чтобы сразу понять, что это булев тип. Имя должно задавать вопрос “да/нет”: `bHasKey`, `bIsSprinting`.                                                                                                                     |
| Булевые переменные для возможностей | `bCanAttack`, `bCanOpenDoor`                | Используй **`bCan…`** + действие в PascalCase. Показывает, что игрок или объект может выполнить действие                                                                                                                     |
| Константы                           | `MaxPlayerSpeed`, `DefaultHealth`           | Константы определяют фиксированные значения, которые не меняются во время игры. Например: `DefaultJumpHeight`, `MaxGravityScale`.                                                                                                   |
| Масивы                              | `InventoryItems`, `EnemyList`               | Используй **существительное** во множественном числе. Например: `CollectedWeapons`, `ActivePlayers`.                                                                                                                                           |
| Ссылки на объекты                   | `WidgetDeadScreenClass`, `WeaponComponent`  | Добавляй тип объекта в **конце** для понятности (`Component`, `Actor`, `Widget`, `Class`).                                                                                                                                                     |
| Таймеры / Делегаты                  | `HealthRegenTimer`, `OnPlayerDeathDelegate` | Для таймеров — с указанием назначения. Для делегатов начинай с `On`.                                                                                                                 |
| UI компоненты                       | `Bor_Crosshair`, `Btn_KickPlayer`           | Используем краткий префикс который берём из название обьекта, пример: (`Border -> Bor_`, `Vertical Box -> VB_`).   

---

## 6. Function Naming Conventions

| Категория                   | Пример                            | Правило / описание                                                                          |
| --------------------------- | --------------------------------- | ------------------------------------------------------------------------------------------- |
| Обычные функции             | `TakeDamage()`, `RestoreHealth()` | Описывай действие, которое делает функция.                     |
| BlueprintCallable           | `OpenDoor()`, `PlayAnimation()`   | Начинай с глагола, чтобы было понятно действие.                                             |
| BlueprintImplementableEvent | `OnDeath()`, `OnItemPickedUp()`   | Начинай с `On`, чтобы показать, что это событие (вызывается движком или другими объектами). |
| Getters / Setters           | `GetHealth()`, `SetHealth()`      | Начинай с `Get` для получения значения и с `Set` для установки значения.                    |


---

## 7. File Content Layout

Хороший пример файла **`.h`**

```cpp
#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExampleActor.generated.h"

UCLASS()
class PROJECTNAME_API AExampleActor : public AActor, public IExampleInterface
{
    GENERATED_BODY()

public:
	/** Client RPCs */
	UFUNCTION(Client, Reliable)
	void Client_Example();

	/** Server RPCs */
	UFUNCTION(Server, Reliable)
	void Server_Example();

	/** Multicast RPCs */
	UFUNCTION(NetMulticast, Reliable)
	void Multi_Example();

	/** Implements */
	virtual void HandleSprint_Implementation(bool bIsSprint) override;

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	/** References */
	UPROPERTY()
	ACharacter* PlayerCharacter = nullptr;

	UFUNCTION()
	void HandleRegenerateHealth();

private:
	/** Timer Handles */
	FTimerHandle RegenTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerAbility", meta = (AllowPrivateAccess = "true"))
	float MaxHealth = 100.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerAbility", meta = (AllowPrivateAccess = "true"))
	float MaxStamina = 100.f;
	
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "PlayerAbility", meta = (AllowPrivateAccess = "true"))
	float Health;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "PlayerAbility", meta = (AllowPrivateAccess = "true"))
	float Stamina;
};
```
