# OpenCode Go для RouteFinder

Проверено 22 ноября 2026 года на macOS: npm-пакет `opencode-ai@1.18.35`,
полный CLI model ID — `opencode-go/gpt-6-luna`.

## Установка

Способ из учебного `OPENCODE_SETUP.md` — установка npm-пакета,
с зафиксированной проверенной версией:

```bash
npm install --global opencode-ai@1.18.35
opencode --version
opencode run --help
```

Ожидаемый вывод версии: `1.18.35`. Для локальной проверки npm-пакет установлен
с `--prefix /tmp/routefinder-opencode-cli`; в Actions используется обычная
глобальная установка той же версии. Проверка Ubuntu ещё не выполнялась.

## Конфигурация и ключ

Проектный `opencode.json` выбирает OpenCode Go, модель GPT-6 Luna и агента
`build`. Ключ подставляется только через `{env:AI_API_KEY}`. Используйте ключ
своей подписки OpenCode Go. Не сохраняйте его в репозитории, `.env` или журнале.
Не копируйте пользовательский `auth.json` в проект.

В Bash можно ввести ключ без отображения и сохранения его в истории команд:

```bash
read -r -s -p 'OpenCode Go API key: ' AI_API_KEY
printf '\n'
export AI_API_KEY
opencode run --agent build --model opencode-go/gpt-6-luna -- \
  'Ответь одним словом: READY' </dev/null
```

Конфигурация использует синтаксис CLI 1.18.35: `provider`,
`provider.opencode-go.options.apiKey` и `permission`. Не заменяйте эти поля
синтаксисом другой версии без повторной проверки. Разрешены чтение, поиск,
редактирование и shell-команды без ручного одобрения, включая доступ к `/tmp`
для сборки. Это рабочий build-агент, а не тестовый read-only агент.
Ограничения изменяемых файлов дополнительно проверяет `scripts/ai/check-changes.py`.

## Выполненная проверка

- `opencode --version`: `1.18.35` для установленного npm-пакета.
- `opencode run --help`: поддерживает `--agent`, `--model`, `--file`, `--format`
  и разделитель `--`, используемые runner-скриптом.
- `opencode debug config`: конфигурация принята, выбран `build`.
- Неинтерактивный запуск с закрытым stdin, моделью `opencode-go/gpt-6-luna`
  и ключом из `AI_API_KEY`: код завершения 0, ответ `READY`.
- В отдельной копии проекта в `/tmp` build-агент прочитал исходник и тест,
  изменил два временных C++-файла через `apply_patch` и выполнил shell-команду.
  Все операции завершились без ручного одобрения.
- Исходный код RouteFinder и его тесты этой проверкой не изменены.
  Регрессия и сквозной AI-fix workflow пока не запускались.

## GitHub Actions

Откройте Settings → Secrets and variables → Actions в репозитории RouteFinder:

| Тип | Имя | Значение |
| --- | --- | --- |
| Repository secret | `AI_API_KEY` | Ваш ключ OpenCode Go |
| Repository variable | `AI_MODEL` | `opencode-go/gpt-6-luna` |
| Repository variable | `OPENCODE_VERSION` | `1.18.35` |

Состояние: эти значения ещё нужно сохранить в настройках GitHub; локальная
конфигурация их автоматически не создаёт. У агента не было доступа к настройкам.

`ai-fix.yml` уже устанавливает `opencode-ai@$OPENCODE_VERSION` и передаёт
`AI_API_KEY` и `AI_MODEL` в `scripts/ai/run-fix.sh`. Ссылок на `VSELLM_API_KEY`
в настройках RouteFinder нет. Переданный через `--model` ID имеет приоритет
над моделью из `opencode.json`.

Официальная документация OpenCode Go:
https://opencode.ai/docs/console/go/
