/*
 * mock_ui.c
 *
 * Copyright (C) 2012, 2013 James Booth <boothj5@gmail.com>
 *
 * This file is part of Profanity.
 *
 * Profanity is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Profanity is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY {} without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Profanity.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <glib.h>
#include <setjmp.h>
#include <cmocka.h>
#include <string.h>

#include "ui/ui.h"
#include "ui/window.h"
#include "tests/helpers.h"

#include "xmpp/bookmark.h"

char output[256];

// Mocks and stubs

static
void _mock_cons_show(const char * const msg, ...)
{
    va_list args;
    va_start(args, msg);
    vsnprintf(output, sizeof(output), msg, args);
    check_expected(output);
    va_end(args);
}

static
void _stub_cons_show(const char * const msg, ...)
{
}

static
void _mock_cons_show_contact_online(PContact contact, Resource *resource, GDateTime *last_activity)
{
    check_expected(contact);
    check_expected(resource);
    check_expected(last_activity);
}

static
void _mock_cons_show_error(const char * const msg, ...)
{
    va_list args;
    va_start(args, msg);
    vsnprintf(output, sizeof(output), msg, args);
    check_expected(output);
    va_end(args);
}

static
void _mock_cons_show_account(ProfAccount *account)
{
    check_expected(account);
}

static
void _mock_cons_show_bookmarks(const GList *list)
{
    check_expected(list);
}

static
void _mock_cons_show_aliases(GList *aliases)
{
    check_expected(aliases);
}

static
void _mock_cons_show_account_list(gchar **accounts)
{
    check_expected(accounts);
}

static
char * _mock_ui_ask_password(void)
{
    return (char *)mock();
}

static
char * _stub_ui_ask_password(void)
{
    return NULL;
}

static
win_type_t _mock_ui_current_win_type(void)
{
    return (win_type_t)mock();
}

static
char * _mock_ui_current_recipeint(void)
{
    return (char *)mock();
}

static
void _mock_ui_handle_error(const char * const err_msg)
{
    check_expected(err_msg);
}

static
void _mock_ui_handle_recipient_error(const char * const recipient,
    const char * const err_msg)
{
    check_expected(recipient);
    check_expected(err_msg);
}

static
void _stub_ui_handle_recipient_error(const char * const recipient,
    const char * const err_msg)
{
}

static
void _mock_ui_handle_recipient_not_found(const char * const recipient,
    const char * const err_msg)
{
    check_expected(recipient);
    check_expected(err_msg);
}

static
void _stub_ui_chat_win_contact_online(PContact contact, Resource *resource, GDateTime *last_activity)
{
}

static
void _stub_ui_handle_recipient_not_found(const char * const recipient, const char * const err_msg)
{
}

static
void _stub_ui_current_refresh(void)
{
}

// bind mocks and stubs

void
mock_cons_show(void)
{
    cons_show = _mock_cons_show;

}

void
mock_cons_show_contact_online(void)
{
    cons_show_contact_online = _mock_cons_show_contact_online;
}

void
stub_ui_chat_win_contact_online(void)
{
    ui_chat_win_contact_online = _stub_ui_chat_win_contact_online;
}

void
mock_cons_show_error(void)
{
    cons_show_error = _mock_cons_show_error;
}

void
mock_cons_show_account(void)
{
    cons_show_account = _mock_cons_show_account;
}

void
mock_cons_show_bookmarks(void)
{
    cons_show_bookmarks = _mock_cons_show_bookmarks;
}

void
mock_cons_show_aliases(void)
{
    cons_show_aliases = _mock_cons_show_aliases;
}

void
mock_cons_show_account_list(void)
{
    cons_show_account_list = _mock_cons_show_account_list;
}

void
mock_ui_ask_password(void)
{
    ui_ask_password = _mock_ui_ask_password;
}

void
mock_ui_current_recipient(void)
{
    ui_current_recipient = _mock_ui_current_recipeint;
}

void
stub_ui_ask_password(void)
{
    ui_ask_password = _stub_ui_ask_password;
}

void
stub_cons_show(void)
{
    cons_show = _stub_cons_show;
}

void
stub_ui_handle_recipient_not_found(void)
{
    ui_handle_recipient_not_found = _stub_ui_handle_recipient_not_found;
}

void
stub_ui_handle_recipient_error(void)
{
    ui_handle_recipient_error = _stub_ui_handle_recipient_error;
}

void
stub_ui_current_refresh(void)
{
    ui_current_refresh = _stub_ui_current_refresh;
}

// expectations

void
expect_cons_show(char *expected)
{
    expect_string(_mock_cons_show, output, expected);
}

void
expect_cons_show_calls(int n)
{
    expect_any_count(_mock_cons_show, output, n);
}

void
expect_cons_show_error(char *expected)
{
    expect_string(_mock_cons_show_error, output, expected);
}

void
expect_cons_show_account(ProfAccount *account)
{
    expect_memory(_mock_cons_show_account, account, account, sizeof(ProfAccount));
}

static gboolean
_cmp_bookmark(Bookmark *bm1, Bookmark *bm2)
{
    if (strcmp(bm1->jid, bm2->jid) != 0) {
        return FALSE;
    }
    if (strcmp(bm1->nick, bm2->nick) != 0) {
        return FALSE;
    }
    if (bm1->autojoin != bm2->autojoin) {
        return FALSE;
    }

    return TRUE;
}

void
expect_cons_show_bookmarks(GList *bookmarks)
{
    glist_set_cmp((GCompareFunc)_cmp_bookmark);
    expect_any(_mock_cons_show_bookmarks, list);
//    expect_check(_mock_cons_show_bookmarks, list, (CheckParameterValue)glist_contents_equal, bookmarks);
}

void
expect_cons_show_account_list(gchar **accounts)
{
    expect_memory(_mock_cons_show_account_list, accounts, accounts, sizeof(accounts));
}

void
expect_cons_show_contact_online(PContact contact, Resource *resource, GDateTime *last_activity)
{
    expect_memory(_mock_cons_show_contact_online, contact, contact, sizeof(contact));
    expect_memory(_mock_cons_show_contact_online, resource, resource, sizeof(Resource));
    if (last_activity == NULL) {
        expect_any(_mock_cons_show_contact_online, last_activity);
    } else {
        expect_memory(_mock_cons_show_contact_online, last_activity, last_activity, sizeof(last_activity));
    }
}

void
expect_cons_show_aliases()
{
    // write a custom checker for the list
    expect_any(_mock_cons_show_aliases, aliases);
}

void
mock_ui_ask_password_returns(char *password)
{
    will_return(_mock_ui_ask_password, strdup(password));
}

void
expect_ui_handle_error(char *err_msg)
{
    ui_handle_error = _mock_ui_handle_error;
    expect_string(_mock_ui_handle_error, err_msg, err_msg);
}

void
expect_ui_handle_recipient_error(char *recipient, char *err_msg)
{
    ui_handle_recipient_error = _mock_ui_handle_recipient_error;
    expect_string(_mock_ui_handle_recipient_error, recipient, recipient);
    expect_string(_mock_ui_handle_recipient_error, err_msg, err_msg);
}

void
expect_ui_handle_recipient_not_found(char *recipient, char *err_msg)
{
    ui_handle_recipient_not_found = _mock_ui_handle_recipient_not_found;
    expect_string(_mock_ui_handle_recipient_not_found, recipient, recipient);
    expect_string(_mock_ui_handle_recipient_not_found, err_msg, err_msg);
}

void
mock_current_win_type(win_type_t type)
{
    ui_current_win_type = _mock_ui_current_win_type;
    will_return(_mock_ui_current_win_type, type);
}

void
ui_current_recipient_returns(char *jid)
{
    will_return(_mock_ui_current_recipeint, jid);
}
