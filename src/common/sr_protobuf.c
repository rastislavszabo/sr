/**
 * @file sr_protobuf.c
 * @author Rastislav Szabo <raszabo@cisco.com>, Lukas Macko <lmacko@cisco.com>,
 *         Milan Lenco <milan.lenco@pantheon.tech>
 * @brief Sysrepo Google Protocol Buffers conversion functions implementation.
 *
 * @copyright
 * Copyright 2016 Cisco Systems, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "sr_protobuf.h"
#include "values_internal.h"

const char *
sr_gpb_operation_name(Sr__Operation operation)
{
    switch (operation) {
    case SR__OPERATION__SESSION_START:
        return "session-start";
    case SR__OPERATION__SESSION_STOP:
        return "session-stop";
    case SR__OPERATION__SESSION_REFRESH:
        return "session-refresh";
    case SR__OPERATION__SESSION_CHECK:
        return "session-check";
    case SR__OPERATION__SESSION_SWITCH_DS:
        return "session-switch-ds";
    case SR__OPERATION__SESSION_SET_OPTS:
        return "session-set-opts";
    case SR__OPERATION__LIST_SCHEMAS:
        return "list-schemas";
    case SR__OPERATION__GET_SCHEMA:
        return "get-schema";
    case SR__OPERATION__MODULE_INSTALL:
        return "module-install";
    case SR__OPERATION__FEATURE_ENABLE:
        return "feature-enable";
    case SR__OPERATION__GET_ITEM:
        return "get-item";
    case SR__OPERATION__GET_ITEMS:
        return "get-items";
    case SR__OPERATION__GET_SUBTREE:
        return "get-subtree";
    case SR__OPERATION__GET_SUBTREES:
        return "get-subtrees";
    case SR__OPERATION__GET_SUBTREE_CHUNK:
        return "get-subtree-chunk";
    case SR__OPERATION__SET_ITEM:
        return "set-item";
    case SR__OPERATION__SET_ITEM_STR:
        return "set-item-str";
    case SR__OPERATION__DELETE_ITEM:
        return "delete-item";
    case SR__OPERATION__MOVE_ITEM:
        return "move-item";
    case SR__OPERATION__VALIDATE:
        return "validate";
    case SR__OPERATION__COMMIT:
        return "commit";
    case SR__OPERATION__DISCARD_CHANGES:
        return "discard-changes";
    case SR__OPERATION__COPY_CONFIG:
        return "copy-config";
    case SR__OPERATION__LOCK:
        return "lock";
    case SR__OPERATION__UNLOCK:
        return "unlock";
    case SR__OPERATION__SUBSCRIBE:
        return "subscribe";
    case SR__OPERATION__UNSUBSCRIBE:
        return "unsubscribe";
    case SR__OPERATION__CHECK_ENABLED_RUNNING:
        return "check-enabled-running";
    case SR__OPERATION__GET_CHANGES:
        return "get changes";
    case SR__OPERATION__DATA_PROVIDE:
        return "data-provide";
    case SR__OPERATION__CHECK_EXEC_PERMISSION:
        return "check-exec-permission";
    case SR__OPERATION__RPC:
        return "rpc";
    case SR__OPERATION__ACTION:
        return "action";
    case SR__OPERATION__UNSUBSCRIBE_DESTINATION:
        return "unsubscribe-destination";
    case SR__OPERATION__COMMIT_TIMEOUT:
        return "commit-timeout";
    case SR__OPERATION__EVENT_NOTIF:
        return "event-notification";
    case SR__OPERATION__EVENT_NOTIF_REPLAY:
        return "event-notification-replay";
    case SR__OPERATION__OPER_DATA_TIMEOUT:
        return "oper-data-timeout";
    case SR__OPERATION__INTERNAL_STATE_DATA:
        return "internal-state-data";
    case SR__OPERATION__NOTIF_STORE_CLEANUP:
        return "notif-store-cleanup";
    case SR__OPERATION__DELAYED_MSG:
        return "delayed-msg";
    case _SR__OPERATION_IS_INT_SIZE:
        return "unknown";
    }
    return "unknown";
}

int
sr_gpb_req_alloc(sr_mem_ctx_t *sr_mem, const Sr__Operation operation, const uint32_t session_id, Sr__Msg **msg_p)
{
    Sr__Msg *msg = NULL;
    Sr__Request *req = NULL;
    ProtobufCMessage *sub_msg = NULL;
    sr_mem_snapshot_t snapshot = { 0, };
    int rc = SR_ERR_OK;

    CHECK_NULL_ARG(msg_p);

    if (sr_mem) {
        sr_mem_snapshot(sr_mem, &snapshot);
    }

    /* initialize Sr__Msg */
    msg = sr_calloc(sr_mem, 1, sizeof(*msg));
    CHECK_NULL_NOMEM_GOTO(msg, rc, error);
    sr__msg__init(msg);
    msg->type = SR__MSG__MSG_TYPE__REQUEST;
    msg->session_id = session_id;

    /* initialize Sr__Resp */
    req = sr_calloc(sr_mem, 1, sizeof(*req));
    CHECK_NULL_NOMEM_GOTO(req, rc, error);
    sr__request__init(req);
    msg->request = req;
    req->operation = operation;

    /* initialize sub-message */
    switch (operation) {
        case SR__OPERATION__SESSION_START:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__SessionStartReq));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__session_start_req__init((Sr__SessionStartReq*)sub_msg);
            req->session_start_req = (Sr__SessionStartReq*)sub_msg;
            break;
        case SR__OPERATION__SESSION_STOP:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__SessionStopReq));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__session_stop_req__init((Sr__SessionStopReq*)sub_msg);
            req->session_stop_req = (Sr__SessionStopReq*)sub_msg;
            break;
        case SR__OPERATION__SESSION_REFRESH:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__SessionRefreshReq));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__session_refresh_req__init((Sr__SessionRefreshReq*)sub_msg);
            req->session_refresh_req = (Sr__SessionRefreshReq*)sub_msg;
            break;
        case SR__OPERATION__SESSION_CHECK:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__SessionCheckReq));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__session_check_req__init((Sr__SessionCheckReq*)sub_msg);
            req->session_check_req = (Sr__SessionCheckReq*)sub_msg;
            break;
        case SR__OPERATION__SESSION_SWITCH_DS:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__SessionSwitchDsReq));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__session_switch_ds_req__init((Sr__SessionSwitchDsReq*)sub_msg);
            req->session_switch_ds_req = (Sr__SessionSwitchDsReq*)sub_msg;
            break;
        case SR__OPERATION__SESSION_SET_OPTS:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__SessionSetOptsReq));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__session_set_opts_req__init((Sr__SessionSetOptsReq*)sub_msg);
            req->session_set_opts_req = (Sr__SessionSetOptsReq*)sub_msg;
            break;
        case SR__OPERATION__LIST_SCHEMAS:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__ListSchemasReq));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__list_schemas_req__init((Sr__ListSchemasReq*)sub_msg);
            req->list_schemas_req = (Sr__ListSchemasReq*)sub_msg;
            break;
        case SR__OPERATION__GET_SCHEMA:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__GetSchemaReq));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__get_schema_req__init((Sr__GetSchemaReq*)sub_msg);
            req->get_schema_req = (Sr__GetSchemaReq*)sub_msg;
            break;
        case SR__OPERATION__FEATURE_ENABLE:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__FeatureEnableReq));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__feature_enable_req__init((Sr__FeatureEnableReq*)sub_msg);
            req->feature_enable_req = (Sr__FeatureEnableReq*)sub_msg;
            break;
        case SR__OPERATION__MODULE_INSTALL:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__ModuleInstallReq));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__module_install_req__init((Sr__ModuleInstallReq*)sub_msg);
            req->module_install_req = (Sr__ModuleInstallReq*)sub_msg;
            break;
        case SR__OPERATION__GET_ITEM:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__GetItemReq));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__get_item_req__init((Sr__GetItemReq*)sub_msg);
            req->get_item_req = (Sr__GetItemReq*)sub_msg;
            break;
        case SR__OPERATION__GET_ITEMS:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__GetItemsReq));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__get_items_req__init((Sr__GetItemsReq*)sub_msg);
            req->get_items_req = (Sr__GetItemsReq*)sub_msg;
            break;
        case SR__OPERATION__GET_SUBTREE:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__GetSubtreeReq));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__get_subtree_req__init((Sr__GetSubtreeReq*)sub_msg);
            req->get_subtree_req = (Sr__GetSubtreeReq*)sub_msg;
            break;
        case SR__OPERATION__GET_SUBTREES:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__GetSubtreesReq));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__get_subtrees_req__init((Sr__GetSubtreesReq*)sub_msg);
            req->get_subtrees_req = (Sr__GetSubtreesReq*)sub_msg;
            break;
        case SR__OPERATION__GET_SUBTREE_CHUNK:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__GetSubtreeChunkReq));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__get_subtree_chunk_req__init((Sr__GetSubtreeChunkReq*)sub_msg);
            req->get_subtree_chunk_req = (Sr__GetSubtreeChunkReq*)sub_msg;
            break;
        case SR__OPERATION__SET_ITEM:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__SetItemReq));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__set_item_req__init((Sr__SetItemReq*)sub_msg);
            req->set_item_req = (Sr__SetItemReq*)sub_msg;
            break;
        case SR__OPERATION__SET_ITEM_STR:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__SetItemStrReq));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__set_item_str_req__init((Sr__SetItemStrReq*)sub_msg);
            req->set_item_str_req = (Sr__SetItemStrReq*)sub_msg;
            break;
        case SR__OPERATION__DELETE_ITEM:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__DeleteItemReq));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__delete_item_req__init((Sr__DeleteItemReq*)sub_msg);
            req->delete_item_req = (Sr__DeleteItemReq*)sub_msg;
            break;
        case SR__OPERATION__MOVE_ITEM:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__MoveItemReq));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__move_item_req__init((Sr__MoveItemReq*)sub_msg);
            req->move_item_req = (Sr__MoveItemReq*)sub_msg;
            break;
        case SR__OPERATION__VALIDATE:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__ValidateReq));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__validate_req__init((Sr__ValidateReq*)sub_msg);
            req->validate_req = (Sr__ValidateReq*)sub_msg;
            break;
        case SR__OPERATION__COMMIT:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__CommitReq));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__commit_req__init((Sr__CommitReq*)sub_msg);
            req->commit_req = (Sr__CommitReq*)sub_msg;
            break;
        case SR__OPERATION__DISCARD_CHANGES:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__DiscardChangesReq));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__discard_changes_req__init((Sr__DiscardChangesReq*)sub_msg);
            req->discard_changes_req = (Sr__DiscardChangesReq*)sub_msg;
            break;
        case SR__OPERATION__COPY_CONFIG:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__CopyConfigReq));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__copy_config_req__init((Sr__CopyConfigReq*)sub_msg);
            req->copy_config_req = (Sr__CopyConfigReq*)sub_msg;
            break;
        case SR__OPERATION__LOCK:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__LockReq));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__lock_req__init((Sr__LockReq*)sub_msg);
            req->lock_req = (Sr__LockReq*)sub_msg;
            break;
        case SR__OPERATION__UNLOCK:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__UnlockReq));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__unlock_req__init((Sr__UnlockReq*)sub_msg);
            req->unlock_req = (Sr__UnlockReq*)sub_msg;
            break;
        case SR__OPERATION__SUBSCRIBE:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__SubscribeReq));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__subscribe_req__init((Sr__SubscribeReq*)sub_msg);
            req->subscribe_req = (Sr__SubscribeReq*)sub_msg;
            break;
        case SR__OPERATION__UNSUBSCRIBE:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__UnsubscribeReq));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__unsubscribe_req__init((Sr__UnsubscribeReq*)sub_msg);
            req->unsubscribe_req = (Sr__UnsubscribeReq*)sub_msg;
            break;
        case SR__OPERATION__CHECK_ENABLED_RUNNING:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__CheckEnabledRunningReq));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__check_enabled_running_req__init((Sr__CheckEnabledRunningReq*)sub_msg);
            req->check_enabled_running_req = (Sr__CheckEnabledRunningReq*)sub_msg;
            break;
        case SR__OPERATION__GET_CHANGES:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__GetChangesReq));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__get_changes_req__init((Sr__GetChangesReq *)sub_msg);
            req->get_changes_req = (Sr__GetChangesReq *)sub_msg;
            break;
        case SR__OPERATION__DATA_PROVIDE:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__DataProvideReq));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__data_provide_req__init((Sr__DataProvideReq*)sub_msg);
            req->data_provide_req = (Sr__DataProvideReq*)sub_msg;
            break;
        case SR__OPERATION__CHECK_EXEC_PERMISSION:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__CheckExecPermReq));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__check_exec_perm_req__init((Sr__CheckExecPermReq*)sub_msg);
            req->check_exec_perm_req = (Sr__CheckExecPermReq*)sub_msg;
            break;
        case SR__OPERATION__RPC:
        case SR__OPERATION__ACTION:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__RPCReq));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__rpcreq__init((Sr__RPCReq*)sub_msg);
            req->rpc_req = (Sr__RPCReq*)sub_msg;
            break;
        case SR__OPERATION__EVENT_NOTIF:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__EventNotifReq));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__event_notif_req__init((Sr__EventNotifReq*)sub_msg);
            req->event_notif_req = (Sr__EventNotifReq*)sub_msg;
            break;
        case SR__OPERATION__EVENT_NOTIF_REPLAY:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__EventNotifReplayReq));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__event_notif_replay_req__init((Sr__EventNotifReplayReq*)sub_msg);
            req->event_notif_replay_req = (Sr__EventNotifReplayReq*)sub_msg;
            break;
        default:
            rc = SR_ERR_UNSUPPORTED;
            goto error;
    }

    /* make association between the message and the context */
    if (sr_mem) {
        ++sr_mem->obj_count;
        msg->_sysrepo_mem_ctx = (uint64_t)sr_mem;
    }

    *msg_p = msg;
    return SR_ERR_OK;

error:
    if (NULL == sr_mem) {
        if (NULL != msg) {
            sr__msg__free_unpacked(msg, NULL);
        }
    } else if (snapshot.sr_mem) {
        sr_mem_restore(&snapshot);
    }
    return rc;
}

int
sr_gpb_resp_alloc(sr_mem_ctx_t *sr_mem, const Sr__Operation operation, const uint32_t session_id, Sr__Msg **msg_p)
{
    Sr__Msg *msg = NULL;
    Sr__Response *resp = NULL;
    ProtobufCMessage *sub_msg = NULL;
    sr_mem_snapshot_t snapshot = { 0, };
    int rc = SR_ERR_OK;

    CHECK_NULL_ARG(msg_p);

    if (sr_mem) {
        sr_mem_snapshot(sr_mem, &snapshot);
    }

    /* initialize Sr__Msg */
    msg = sr_calloc(sr_mem, 1, sizeof(*msg));
    CHECK_NULL_NOMEM_GOTO(msg, rc, error);
    sr__msg__init(msg);
    msg->type = SR__MSG__MSG_TYPE__RESPONSE;
    msg->session_id = session_id;

    /* initialize Sr__Resp */
    resp = sr_calloc(sr_mem, 1, sizeof(*resp));
    CHECK_NULL_NOMEM_GOTO(resp, rc, error);
    sr__response__init(resp);
    msg->response = resp;
    resp->operation = operation;
    resp->result = SR_ERR_OK;

    /* initialize sub-message */
    switch (operation) {
        case SR__OPERATION__SESSION_START:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__SessionStartResp));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__session_start_resp__init((Sr__SessionStartResp*)sub_msg);
            resp->session_start_resp = (Sr__SessionStartResp*)sub_msg;
            break;
        case SR__OPERATION__SESSION_STOP:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__SessionStopResp));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__session_stop_resp__init((Sr__SessionStopResp*)sub_msg);
            resp->session_stop_resp = (Sr__SessionStopResp*)sub_msg;
            break;
        case SR__OPERATION__SESSION_REFRESH:
           sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__SessionRefreshResp));
           CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
           sr__session_refresh_resp__init((Sr__SessionRefreshResp*)sub_msg);
           resp->session_refresh_resp = (Sr__SessionRefreshResp*)sub_msg;
           break;
        case SR__OPERATION__SESSION_CHECK:
           sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__SessionCheckResp));
           CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
           sr__session_check_resp__init((Sr__SessionCheckResp*)sub_msg);
           resp->session_check_resp = (Sr__SessionCheckResp*)sub_msg;
           break;
        case SR__OPERATION__SESSION_SWITCH_DS:
           sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__SessionSwitchDsResp));
           CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
           sr__session_switch_ds_resp__init((Sr__SessionSwitchDsResp*)sub_msg);
           resp->session_switch_ds_resp = (Sr__SessionSwitchDsResp*)sub_msg;
           break;
        case SR__OPERATION__SESSION_SET_OPTS:
           sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__SessionSetOptsResp));
           CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
           sr__session_set_opts_resp__init((Sr__SessionSetOptsResp*)sub_msg);
           resp->session_set_opts_resp = (Sr__SessionSetOptsResp*)sub_msg;
           break;
        case SR__OPERATION__LIST_SCHEMAS:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__ListSchemasResp));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__list_schemas_resp__init((Sr__ListSchemasResp*)sub_msg);
            resp->list_schemas_resp = (Sr__ListSchemasResp*)sub_msg;
            break;
        case SR__OPERATION__GET_SCHEMA:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__GetSchemaResp));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__get_schema_resp__init((Sr__GetSchemaResp*)sub_msg);
            resp->get_schema_resp = (Sr__GetSchemaResp*)sub_msg;
            break;
        case SR__OPERATION__GET_ITEM:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__GetItemResp));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__get_item_resp__init((Sr__GetItemResp*)sub_msg);
            resp->get_item_resp = (Sr__GetItemResp*)sub_msg;
            break;
        case SR__OPERATION__FEATURE_ENABLE:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__FeatureEnableResp));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__feature_enable_resp__init((Sr__FeatureEnableResp*)sub_msg);
            resp->feature_enable_resp = (Sr__FeatureEnableResp*)sub_msg;
            break;
        case SR__OPERATION__MODULE_INSTALL:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__ModuleInstallResp));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__module_install_resp__init((Sr__ModuleInstallResp*)sub_msg);
            resp->module_install_resp = (Sr__ModuleInstallResp*)sub_msg;
            break;
        case SR__OPERATION__GET_ITEMS:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__GetItemsResp));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__get_items_resp__init((Sr__GetItemsResp*)sub_msg);
            resp->get_items_resp = (Sr__GetItemsResp*)sub_msg;
            break;
        case SR__OPERATION__GET_SUBTREE:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__GetSubtreeResp));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__get_subtree_resp__init((Sr__GetSubtreeResp*)sub_msg);
            resp->get_subtree_resp = (Sr__GetSubtreeResp*)sub_msg;
            break;
        case SR__OPERATION__GET_SUBTREES:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__GetSubtreesResp));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__get_subtrees_resp__init((Sr__GetSubtreesResp*)sub_msg);
            resp->get_subtrees_resp = (Sr__GetSubtreesResp*)sub_msg;
            break;
        case SR__OPERATION__GET_SUBTREE_CHUNK:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__GetSubtreeChunkResp));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__get_subtree_chunk_resp__init((Sr__GetSubtreeChunkResp*)sub_msg);
            resp->get_subtree_chunk_resp = (Sr__GetSubtreeChunkResp*)sub_msg;
            break;
        case SR__OPERATION__SET_ITEM:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__SetItemResp));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__set_item_resp__init((Sr__SetItemResp*)sub_msg);
            resp->set_item_resp = (Sr__SetItemResp*)sub_msg;
            break;
        case SR__OPERATION__SET_ITEM_STR:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__SetItemStrResp));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__set_item_str_resp__init((Sr__SetItemStrResp*)sub_msg);
            resp->set_item_str_resp = (Sr__SetItemStrResp*)sub_msg;
            break;
        case SR__OPERATION__DELETE_ITEM:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__DeleteItemResp));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__delete_item_resp__init((Sr__DeleteItemResp*)sub_msg);
            resp->delete_item_resp = (Sr__DeleteItemResp*)sub_msg;
            break;
        case SR__OPERATION__MOVE_ITEM:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__MoveItemResp));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__move_item_resp__init((Sr__MoveItemResp*)sub_msg);
            resp->move_item_resp = (Sr__MoveItemResp*)sub_msg;
            break;
        case SR__OPERATION__VALIDATE:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__ValidateResp));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__validate_resp__init((Sr__ValidateResp*)sub_msg);
            resp->validate_resp = (Sr__ValidateResp*)sub_msg;
            break;
        case SR__OPERATION__COMMIT:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__CommitResp));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__commit_resp__init((Sr__CommitResp*)sub_msg);
            resp->commit_resp = (Sr__CommitResp*)sub_msg;
            break;
        case SR__OPERATION__DISCARD_CHANGES:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__DiscardChangesResp));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__discard_changes_resp__init((Sr__DiscardChangesResp*)sub_msg);
            resp->discard_changes_resp = (Sr__DiscardChangesResp*)sub_msg;
            break;
        case SR__OPERATION__COPY_CONFIG:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__CopyConfigResp));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__copy_config_resp__init((Sr__CopyConfigResp*)sub_msg);
            resp->copy_config_resp = (Sr__CopyConfigResp*)sub_msg;
            break;
        case SR__OPERATION__LOCK:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__LockResp));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__lock_resp__init((Sr__LockResp*)sub_msg);
            resp->lock_resp = (Sr__LockResp*)sub_msg;
            break;
        case SR__OPERATION__UNLOCK:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__UnlockResp));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__unlock_resp__init((Sr__UnlockResp*)sub_msg);
            resp->unlock_resp = (Sr__UnlockResp*)sub_msg;
            break;
        case SR__OPERATION__SUBSCRIBE:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__SubscribeResp));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__subscribe_resp__init((Sr__SubscribeResp*)sub_msg);
            resp->subscribe_resp = (Sr__SubscribeResp*)sub_msg;
            break;
        case SR__OPERATION__UNSUBSCRIBE:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__UnsubscribeResp));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__unsubscribe_resp__init((Sr__UnsubscribeResp*)sub_msg);
            resp->unsubscribe_resp = (Sr__UnsubscribeResp*)sub_msg;
            break;
        case SR__OPERATION__CHECK_ENABLED_RUNNING:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__CheckEnabledRunningResp));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__check_enabled_running_resp__init((Sr__CheckEnabledRunningResp*)sub_msg);
            resp->check_enabled_running_resp = (Sr__CheckEnabledRunningResp*)sub_msg;
            break;
        case SR__OPERATION__GET_CHANGES:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__GetChangesResp));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__get_changes_resp__init((Sr__GetChangesResp*)sub_msg);
            resp->get_changes_resp = (Sr__GetChangesResp*)sub_msg;
            break;
        case SR__OPERATION__DATA_PROVIDE:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__DataProvideResp));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__data_provide_resp__init((Sr__DataProvideResp*)sub_msg);
            resp->data_provide_resp = (Sr__DataProvideResp*)sub_msg;
            break;
        case SR__OPERATION__CHECK_EXEC_PERMISSION:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__CheckExecPermResp));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__check_exec_perm_resp__init((Sr__CheckExecPermResp*)sub_msg);
            resp->check_exec_perm_resp = (Sr__CheckExecPermResp*)sub_msg;
            break;
        case SR__OPERATION__RPC:
        case SR__OPERATION__ACTION:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__RPCResp));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__rpcresp__init((Sr__RPCResp*)sub_msg);
            resp->rpc_resp = (Sr__RPCResp*)sub_msg;
            break;
        case SR__OPERATION__EVENT_NOTIF:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__EventNotifResp));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__event_notif_resp__init((Sr__EventNotifResp*)sub_msg);
            resp->event_notif_resp = (Sr__EventNotifResp*)sub_msg;
            break;
        case SR__OPERATION__EVENT_NOTIF_REPLAY:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__EventNotifReplayResp));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__event_notif_replay_resp__init((Sr__EventNotifReplayResp*)sub_msg);
            resp->event_notif_replay_resp = (Sr__EventNotifReplayResp*)sub_msg;
            break;
        default:
            rc = SR_ERR_UNSUPPORTED;
            goto error;
    }

    /* make association between the message and the context */
    if (sr_mem) {
        ++sr_mem->obj_count;
        msg->_sysrepo_mem_ctx = (uint64_t)sr_mem;
    }

    *msg_p = msg;
    return SR_ERR_OK;

error:
    if (NULL == sr_mem) {
        if (NULL != msg) {
            sr__msg__free_unpacked(msg, NULL);
        }
    } else if (snapshot.sr_mem) {
        sr_mem_restore(&snapshot);
    }
    return rc;
}

int
sr_gpb_notif_alloc(sr_mem_ctx_t *sr_mem, const Sr__SubscriptionType type, const char *destination, const uint32_t subscription_id, Sr__Msg **msg_p)
{
    Sr__Msg *msg = NULL;
    Sr__Notification *notif = NULL;
    ProtobufCMessage *sub_msg = NULL;
    sr_mem_snapshot_t snapshot = { 0, };
    int rc = SR_ERR_OK;

    CHECK_NULL_ARG2(destination, msg_p);

    if (sr_mem) {
        sr_mem_snapshot(sr_mem, &snapshot);
    }

    /* initialize Sr__Msg */
    msg = sr_calloc(sr_mem, 1, sizeof(*msg));
    CHECK_NULL_NOMEM_GOTO(msg, rc, error);
    sr__msg__init(msg);
    msg->type = SR__MSG__MSG_TYPE__NOTIFICATION;
    msg->session_id = 0;

    /* initialize Sr__Notification */
    notif = sr_calloc(sr_mem, 1, sizeof(*notif));
    CHECK_NULL_NOMEM_GOTO(notif, rc, error);
    sr__notification__init(notif);
    msg->notification = notif;

    notif->type = type;
    notif->subscription_id = subscription_id;

    notif->destination_address = strdup(destination);
    CHECK_NULL_NOMEM_GOTO(notif->destination_address, rc, error);

    /* initialize sub-message */
    switch (type) {
        case SR__SUBSCRIPTION_TYPE__MODULE_INSTALL_SUBS:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__ModuleInstallNotification));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__module_install_notification__init((Sr__ModuleInstallNotification*)sub_msg);
            notif->module_install_notif = (Sr__ModuleInstallNotification*)sub_msg;
            break;
        case SR__SUBSCRIPTION_TYPE__FEATURE_ENABLE_SUBS:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__FeatureEnableNotification));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__feature_enable_notification__init((Sr__FeatureEnableNotification*)sub_msg);
            notif->feature_enable_notif = (Sr__FeatureEnableNotification*)sub_msg;
            break;
        case SR__SUBSCRIPTION_TYPE__MODULE_CHANGE_SUBS:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__ModuleChangeNotification));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__module_change_notification__init((Sr__ModuleChangeNotification*)sub_msg);
            notif->module_change_notif = (Sr__ModuleChangeNotification*)sub_msg;
            break;
        case SR__SUBSCRIPTION_TYPE__SUBTREE_CHANGE_SUBS:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__SubtreeChangeNotification));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__subtree_change_notification__init((Sr__SubtreeChangeNotification*)sub_msg);
            notif->subtree_change_notif = (Sr__SubtreeChangeNotification*)sub_msg;
            break;
        case SR__SUBSCRIPTION_TYPE__HELLO_SUBS:
        case SR__SUBSCRIPTION_TYPE__COMMIT_END_SUBS:
            /* no sub-message */
            break;
        default:
            rc = SR_ERR_UNSUPPORTED;
            goto error;
    }

    /* make association between the message and the context */
    if (sr_mem) {
        ++sr_mem->obj_count;
        msg->_sysrepo_mem_ctx = (uint64_t)sr_mem;
    }

    *msg_p = msg;
    return SR_ERR_OK;

error:
    if (NULL == sr_mem) {
        if (NULL != msg) {
            sr__msg__free_unpacked(msg, NULL);
        }
    } else if (snapshot.sr_mem) {
        sr_mem_restore(&snapshot);
    }
    return rc;
}

int
sr_gpb_notif_ack_alloc(sr_mem_ctx_t *sr_mem, Sr__Msg *notification, Sr__Msg **msg_p)
{
    Sr__Msg *msg = NULL;
    Sr__NotificationAck *notif_ack = NULL;
    sr_mem_snapshot_t snapshot = { 0, };
    int rc = SR_ERR_OK;

    CHECK_NULL_ARG3(notification, notification->notification, msg_p);

    if (sr_mem) {
        sr_mem_snapshot(sr_mem, &snapshot);
    }

    /* initialize Sr__Msg */
    msg = sr_calloc(sr_mem, 1, sizeof(*msg));
    CHECK_NULL_NOMEM_GOTO(msg, rc, error);
    sr__msg__init(msg);
    msg->type = SR__MSG__MSG_TYPE__NOTIFICATION_ACK;
    msg->session_id = 0;

    /* initialize Sr__NotificationAck */
    notif_ack = sr_calloc(sr_mem, 1, sizeof(*notif_ack));
    CHECK_NULL_NOMEM_GOTO(notif_ack, rc, error);
    sr__notification_ack__init(notif_ack);
    msg->notification_ack = notif_ack;

    notif_ack->notif = notification->notification;

    /* make association between the message and the context */
    if (sr_mem) {
        ++sr_mem->obj_count;
        msg->_sysrepo_mem_ctx = (uint64_t)sr_mem;
    }

    *msg_p = msg;
    return SR_ERR_OK;

error:
    if (NULL == sr_mem) {
        if (NULL != msg) {
            sr__msg__free_unpacked(msg, NULL);
        }
    } else if (snapshot.sr_mem) {
        sr_mem_restore(&snapshot);
    }
    return rc;
}

int
sr_gpb_internal_req_alloc(sr_mem_ctx_t *sr_mem, const Sr__Operation operation, Sr__Msg **msg_p)
{
    Sr__Msg *msg = NULL;
    Sr__InternalRequest *req = NULL;
    ProtobufCMessage *sub_msg = NULL;
    sr_mem_snapshot_t snapshot = { 0, };
    int rc = SR_ERR_OK;

    CHECK_NULL_ARG(msg_p);

    if (sr_mem) {
        sr_mem_snapshot(sr_mem, &snapshot);
    }

    /* initialize Sr__Msg */
    msg = sr_calloc(sr_mem, 1, sizeof(*msg));
    CHECK_NULL_NOMEM_GOTO(msg, rc, error);
    sr__msg__init(msg);
    msg->type = SR__MSG__MSG_TYPE__INTERNAL_REQUEST;
    msg->session_id = 0;

    /* initialize Sr__InternalRequest */
    req = sr_calloc(sr_mem, 1, sizeof(*req));
    CHECK_NULL_NOMEM_GOTO(req, rc, error);
    sr__internal_request__init(req);
    msg->internal_request = req;

    msg->internal_request->operation = operation;

    /* initialize sub-message */
    switch (operation) {
        case SR__OPERATION__UNSUBSCRIBE_DESTINATION:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__UnsubscribeDestinationReq));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__unsubscribe_destination_req__init((Sr__UnsubscribeDestinationReq*)sub_msg);
            req->unsubscribe_dst_req = (Sr__UnsubscribeDestinationReq*)sub_msg;
            break;
        case SR__OPERATION__COMMIT_TIMEOUT:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__CommitTimeoutReq));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__commit_timeout_req__init((Sr__CommitTimeoutReq*)sub_msg);
            req->commit_timeout_req = (Sr__CommitTimeoutReq*)sub_msg;
            break;
        case SR__OPERATION__OPER_DATA_TIMEOUT:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__OperDataTimeoutReq));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__oper_data_timeout_req__init((Sr__OperDataTimeoutReq*)sub_msg);
            req->oper_data_timeout_req = (Sr__OperDataTimeoutReq*)sub_msg;
            break;
        case SR__OPERATION__INTERNAL_STATE_DATA:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__InternalStateDataReq));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__internal_state_data_req__init((Sr__InternalStateDataReq*)sub_msg);
            req->internal_state_data_req = (Sr__InternalStateDataReq*) sub_msg;
            break;
        case SR__OPERATION__NOTIF_STORE_CLEANUP:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__NotifStoreCleanupReq));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__notif_store_cleanup_req__init((Sr__NotifStoreCleanupReq*)sub_msg);
            req->notif_store_cleanup_req = (Sr__NotifStoreCleanupReq*) sub_msg;
            break;
        case SR__OPERATION__DELAYED_MSG:
            sub_msg = sr_calloc(sr_mem, 1, sizeof(Sr__DelayedMsgReq));
            CHECK_NULL_NOMEM_GOTO(sub_msg, rc, error);
            sr__delayed_msg_req__init((Sr__DelayedMsgReq*)sub_msg);
            req->delayed_msg_req = (Sr__DelayedMsgReq*) sub_msg;
            break;
        default:
            break;
    }

    /* make association between the message and the context */
    if (sr_mem) {
        ++sr_mem->obj_count;
        msg->_sysrepo_mem_ctx = (uint64_t)sr_mem;
    }

    *msg_p = msg;
    return SR_ERR_OK;

error:
    if (NULL == sr_mem) {
        if (NULL != msg) {
            sr__msg__free_unpacked(msg, NULL);
        }
    } else if (snapshot.sr_mem) {
        sr_mem_restore(&snapshot);
    }
    return rc;
}

int
sr_gpb_msg_validate(const Sr__Msg *msg, const Sr__Msg__MsgType type, const Sr__Operation operation)
{
    CHECK_NULL_ARG(msg);

    if (SR__MSG__MSG_TYPE__REQUEST == type) {
        /* request */
        CHECK_NULL_RETURN(msg->request, SR_ERR_MALFORMED_MSG);
        if (msg->request->operation != operation) {
            return SR_ERR_MALFORMED_MSG;
        }
        switch (operation) {
            case SR__OPERATION__SESSION_START:
                CHECK_NULL_RETURN(msg->request->session_start_req, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__SESSION_STOP:
                CHECK_NULL_RETURN(msg->request->session_stop_req, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__SESSION_REFRESH:
                CHECK_NULL_RETURN(msg->request->session_refresh_req, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__SESSION_CHECK:
                CHECK_NULL_RETURN(msg->request->session_check_req, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__SESSION_SWITCH_DS:
                CHECK_NULL_RETURN(msg->request->session_switch_ds_req, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__SESSION_SET_OPTS:
                CHECK_NULL_RETURN(msg->request->session_set_opts_req, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__LIST_SCHEMAS:
                CHECK_NULL_RETURN(msg->request->list_schemas_req, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__GET_SCHEMA:
                CHECK_NULL_RETURN(msg->request->get_schema_req, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__FEATURE_ENABLE:
                CHECK_NULL_RETURN(msg->request->feature_enable_req, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__MODULE_INSTALL:
                CHECK_NULL_RETURN(msg->request->module_install_req, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__GET_ITEM:
                CHECK_NULL_RETURN(msg->request->get_item_req, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__GET_ITEMS:
                CHECK_NULL_RETURN(msg->request->get_items_req, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__GET_SUBTREE:
                CHECK_NULL_RETURN(msg->request->get_subtree_req, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__GET_SUBTREES:
                CHECK_NULL_RETURN(msg->request->get_subtrees_req, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__GET_SUBTREE_CHUNK:
                CHECK_NULL_RETURN(msg->request->get_subtree_chunk_req, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__SET_ITEM:
                CHECK_NULL_RETURN(msg->request->set_item_req, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__SET_ITEM_STR:
                CHECK_NULL_RETURN(msg->request->set_item_str_req, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__DELETE_ITEM:
                CHECK_NULL_RETURN(msg->request->delete_item_req, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__MOVE_ITEM:
                CHECK_NULL_RETURN(msg->request->move_item_req, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__VALIDATE:
                CHECK_NULL_RETURN(msg->request->validate_req, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__COMMIT:
                CHECK_NULL_RETURN(msg->request->commit_req, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__DISCARD_CHANGES:
                CHECK_NULL_RETURN(msg->request->discard_changes_req, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__COPY_CONFIG:
                CHECK_NULL_RETURN(msg->request->copy_config_req, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__LOCK:
                CHECK_NULL_RETURN(msg->request->lock_req, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__UNLOCK:
                CHECK_NULL_RETURN(msg->request->unlock_req, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__SUBSCRIBE:
                CHECK_NULL_RETURN(msg->request->subscribe_req, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__UNSUBSCRIBE:
                CHECK_NULL_RETURN(msg->request->unsubscribe_req, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__CHECK_ENABLED_RUNNING:
                CHECK_NULL_RETURN(msg->request->check_enabled_running_req, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__GET_CHANGES:
                CHECK_NULL_RETURN(msg->request->get_changes_req, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__DATA_PROVIDE:
                CHECK_NULL_RETURN(msg->request->data_provide_req, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__CHECK_EXEC_PERMISSION:
                CHECK_NULL_RETURN(msg->request->check_exec_perm_req, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__RPC:
            case SR__OPERATION__ACTION:
                CHECK_NULL_RETURN(msg->request->rpc_req, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__EVENT_NOTIF:
                CHECK_NULL_RETURN(msg->request->event_notif_req, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__EVENT_NOTIF_REPLAY:
                CHECK_NULL_RETURN(msg->request->event_notif_replay_req, SR_ERR_MALFORMED_MSG);
                break;
            default:
                return SR_ERR_MALFORMED_MSG;
        }
    } else if (SR__MSG__MSG_TYPE__RESPONSE == type) {
        /* response */
        CHECK_NULL_RETURN(msg->response, SR_ERR_MALFORMED_MSG);
        if (msg->response->operation != operation) {
            return SR_ERR_MALFORMED_MSG;
        }
        switch (operation) {
            case SR__OPERATION__SESSION_START:
                CHECK_NULL_RETURN(msg->response->session_start_resp, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__SESSION_STOP:
                CHECK_NULL_RETURN(msg->response->session_stop_resp, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__SESSION_REFRESH:
                CHECK_NULL_RETURN(msg->response->session_refresh_resp, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__SESSION_CHECK:
                CHECK_NULL_RETURN(msg->response->session_check_resp, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__SESSION_SWITCH_DS:
                CHECK_NULL_RETURN(msg->response->session_switch_ds_resp, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__SESSION_SET_OPTS:
                CHECK_NULL_RETURN(msg->response->session_set_opts_resp, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__LIST_SCHEMAS:
                CHECK_NULL_RETURN(msg->response->list_schemas_resp, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__GET_SCHEMA:
                CHECK_NULL_RETURN(msg->response->get_schema_resp, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__FEATURE_ENABLE:
                CHECK_NULL_RETURN(msg->response->feature_enable_resp, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__MODULE_INSTALL:
                CHECK_NULL_RETURN(msg->response->module_install_resp, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__GET_ITEM:
                CHECK_NULL_RETURN(msg->response->get_item_resp, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__GET_ITEMS:
                CHECK_NULL_RETURN(msg->response->get_items_resp, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__GET_SUBTREE:
                CHECK_NULL_RETURN(msg->response->get_subtree_resp, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__GET_SUBTREES:
                CHECK_NULL_RETURN(msg->response->get_subtrees_resp, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__GET_SUBTREE_CHUNK:
                CHECK_NULL_RETURN(msg->response->get_subtree_chunk_resp, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__SET_ITEM:
                CHECK_NULL_RETURN(msg->response->set_item_resp, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__SET_ITEM_STR:
                CHECK_NULL_RETURN(msg->response->set_item_str_resp, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__DELETE_ITEM:
                CHECK_NULL_RETURN(msg->response->delete_item_resp, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__MOVE_ITEM:
                CHECK_NULL_RETURN(msg->response->move_item_resp, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__VALIDATE:
                CHECK_NULL_RETURN(msg->response->validate_resp, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__COMMIT:
                CHECK_NULL_RETURN(msg->response->commit_resp, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__DISCARD_CHANGES:
                CHECK_NULL_RETURN(msg->response->discard_changes_resp, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__COPY_CONFIG:
                CHECK_NULL_RETURN(msg->response->copy_config_resp, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__LOCK:
                CHECK_NULL_RETURN(msg->response->lock_resp, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__UNLOCK:
                CHECK_NULL_RETURN(msg->response->unlock_resp, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__SUBSCRIBE:
                CHECK_NULL_RETURN(msg->response->subscribe_resp, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__UNSUBSCRIBE:
                CHECK_NULL_RETURN(msg->response->unsubscribe_resp, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__CHECK_ENABLED_RUNNING:
                CHECK_NULL_RETURN(msg->response->check_enabled_running_resp, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__GET_CHANGES:
                CHECK_NULL_RETURN(msg->response->get_changes_resp, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__DATA_PROVIDE:
                CHECK_NULL_RETURN(msg->response->data_provide_resp, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__CHECK_EXEC_PERMISSION:
                CHECK_NULL_RETURN(msg->response->check_exec_perm_resp, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__RPC:
            case SR__OPERATION__ACTION:
                CHECK_NULL_RETURN(msg->response->rpc_resp, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__EVENT_NOTIF:
                CHECK_NULL_RETURN(msg->response->event_notif_resp, SR_ERR_MALFORMED_MSG);
                break;
            case SR__OPERATION__EVENT_NOTIF_REPLAY:
                CHECK_NULL_RETURN(msg->response->event_notif_replay_resp, SR_ERR_MALFORMED_MSG);
                break;
            default:
                return SR_ERR_MALFORMED_MSG;
        }
    } else {
        /* unknown operation */
        return SR_ERR_MALFORMED_MSG;
    }

    return SR_ERR_OK;
}

int
sr_gpb_msg_validate_notif(const Sr__Msg *msg, const Sr__SubscriptionType type)
{
    CHECK_NULL_ARG(msg);

    if (SR__MSG__MSG_TYPE__NOTIFICATION == msg->type) {
        CHECK_NULL_RETURN(msg->notification, SR_ERR_MALFORMED_MSG);
        if ((msg->notification->type != SR__SUBSCRIPTION_TYPE__HELLO_SUBS) &&
                (msg->notification->type != SR__SUBSCRIPTION_TYPE__COMMIT_END_SUBS) &&
                (msg->notification->type != type)) {
            return SR_ERR_MALFORMED_MSG;
        }
        switch (msg->notification->type) {
            case SR__SUBSCRIPTION_TYPE__MODULE_INSTALL_SUBS:
                CHECK_NULL_RETURN(msg->notification->module_install_notif, SR_ERR_MALFORMED_MSG);
                break;
            case SR__SUBSCRIPTION_TYPE__FEATURE_ENABLE_SUBS:
                CHECK_NULL_RETURN(msg->notification->feature_enable_notif, SR_ERR_MALFORMED_MSG);
                break;
            case SR__SUBSCRIPTION_TYPE__MODULE_CHANGE_SUBS:
                CHECK_NULL_RETURN(msg->notification->module_change_notif, SR_ERR_MALFORMED_MSG);
                break;
            case SR__SUBSCRIPTION_TYPE__SUBTREE_CHANGE_SUBS:
                CHECK_NULL_RETURN(msg->notification->subtree_change_notif, SR_ERR_MALFORMED_MSG);
                break;
            case SR__SUBSCRIPTION_TYPE__HELLO_SUBS:
            case SR__SUBSCRIPTION_TYPE__COMMIT_END_SUBS:
                break;
            default:
                return SR_ERR_MALFORMED_MSG;
        }
    } else {
        return SR_ERR_MALFORMED_MSG;
    }

    return SR_ERR_OK;
}

static int
sr_set_val_t_type_in_gpb(const sr_val_t *value, Sr__Value *gpb_value){
    CHECK_NULL_ARG2(value, gpb_value);
    int rc = SR_ERR_OK;
    switch (value->type) {
    case SR_LIST_T:
        gpb_value->type = SR__VALUE__TYPES__LIST;
        break;
    case SR_CONTAINER_T:
        gpb_value->type = SR__VALUE__TYPES__CONTAINER;
        break;
    case SR_CONTAINER_PRESENCE_T:
        gpb_value->type = SR__VALUE__TYPES__CONTAINER_PRESENCE;
        break;
    case SR_LEAF_EMPTY_T:
        gpb_value->type = SR__VALUE__TYPES__LEAF_EMPTY;
        break;
    case SR_BINARY_T:
        gpb_value->type = SR__VALUE__TYPES__BINARY;
        break;
    case SR_BITS_T:
        gpb_value->type = SR__VALUE__TYPES__BITS;
        break;
    case SR_BOOL_T:
        gpb_value->type = SR__VALUE__TYPES__BOOL;
        break;
    case SR_DECIMAL64_T:
        gpb_value->type = SR__VALUE__TYPES__DECIMAL64;
        break;
    case SR_ENUM_T:
        gpb_value->type = SR__VALUE__TYPES__ENUM;
        break;
    case SR_IDENTITYREF_T:
        gpb_value->type = SR__VALUE__TYPES__IDENTITYREF;
        break;
    case SR_INSTANCEID_T:
        gpb_value->type = SR__VALUE__TYPES__INSTANCEID;
        break;
    case SR_INT8_T:
        gpb_value->type = SR__VALUE__TYPES__INT8;
        break;
    case SR_INT16_T:
        gpb_value->type = SR__VALUE__TYPES__INT16;
        break;
    case SR_INT32_T:
        gpb_value->type = SR__VALUE__TYPES__INT32;
        break;
    case SR_INT64_T:
        gpb_value->type = SR__VALUE__TYPES__INT64;
        break;
    case SR_STRING_T:
        gpb_value->type = SR__VALUE__TYPES__STRING;
        break;
    case SR_UINT8_T:
        gpb_value->type = SR__VALUE__TYPES__UINT8;
        break;
    case SR_UINT16_T:
        gpb_value->type = SR__VALUE__TYPES__UINT16;
        break;
    case SR_UINT32_T:
        gpb_value->type = SR__VALUE__TYPES__UINT32;
        break;
    case SR_UINT64_T:
        gpb_value->type = SR__VALUE__TYPES__UINT64;
        break;
    case SR_ANYXML_T:
        gpb_value->type = SR__VALUE__TYPES__ANYXML;
        break;
    case SR_ANYDATA_T:
        gpb_value->type = SR__VALUE__TYPES__ANYDATA;
        break;

    default:
        SR_LOG_ERR("Type can not be mapped to gpb type '%s' type %d", value->xpath, value->type);
        return SR_ERR_INTERNAL;
    }

    return rc;
}

/**
 * @brief Copies data from sr_val_t to GPB message. Makes shallow copy if the value
 * is allocated inside Sysrepo memory context, otherwise a deep copy.
 */
static int
sr_set_val_t_value_in_gpb(const sr_val_t *value, Sr__Value *gpb_value){
    CHECK_NULL_ARG2(value, gpb_value);

    if (NULL != value->xpath) {
        if (value->_sr_mem) {
            gpb_value->xpath = value->xpath;
        } else {
            gpb_value->xpath = strdup(value->xpath);
            CHECK_NULL_NOMEM_RETURN(gpb_value->xpath);
        }
    }

    gpb_value->dflt = value->dflt;
    switch (value->type) {
    case SR_LIST_T:
    case SR_CONTAINER_T:
    case SR_CONTAINER_PRESENCE_T:
    case SR_LEAF_EMPTY_T:
        return SR_ERR_OK;
    case SR_BINARY_T:
        if (value->_sr_mem) {
            gpb_value->binary_val = value->data.binary_val;
        } else if (NULL != value->data.binary_val) {
            gpb_value->binary_val = strdup(value->data.binary_val);
            CHECK_NULL_NOMEM_RETURN(gpb_value->binary_val);
        }
        return SR_ERR_OK;
    case SR_BITS_T:
        if (value->_sr_mem) {
            gpb_value->bits_val = value->data.bits_val;
        } else if (NULL != value->data.bits_val) {
            gpb_value->bits_val = strdup(value->data.bits_val);
            CHECK_NULL_NOMEM_RETURN(gpb_value->bits_val);
        }
        return SR_ERR_OK;
    case SR_BOOL_T:
        gpb_value->bool_val = value->data.bool_val;
        gpb_value->has_bool_val = true;
        return SR_ERR_OK;
    case SR_DECIMAL64_T:
        gpb_value->decimal64_val = value->data.decimal64_val;
        gpb_value->has_decimal64_val = true;
        return SR_ERR_OK;
    case SR_ENUM_T:
        if (value->_sr_mem) {
            gpb_value->enum_val = value->data.enum_val;
        } else if (NULL != value->data.enum_val) {
            gpb_value->enum_val = strdup(value->data.enum_val);
            CHECK_NULL_NOMEM_RETURN(gpb_value->enum_val);
        }
        return SR_ERR_OK;
    case SR_IDENTITYREF_T:
        if (value->_sr_mem) {
            gpb_value->identityref_val = value->data.identityref_val;
        } else if (NULL != value->data.identityref_val) {
            gpb_value->identityref_val = strdup(value->data.identityref_val);
            CHECK_NULL_NOMEM_RETURN(gpb_value->identityref_val);
        }
        return SR_ERR_OK;
    case SR_INSTANCEID_T:
        if (value->_sr_mem) {
            gpb_value->instanceid_val = value->data.instanceid_val;
        } else if (NULL != value->data.instanceid_val) {
            gpb_value->instanceid_val = strdup(value->data.instanceid_val);
            CHECK_NULL_NOMEM_RETURN(gpb_value->instanceid_val);
        }
        return SR_ERR_OK;
    case SR_INT8_T:
        gpb_value->int8_val = value->data.int8_val;
        gpb_value->has_int8_val = true;
        break;
    case SR_INT16_T:
        gpb_value->int16_val = value->data.int16_val;
        gpb_value->has_int16_val = true;
        break;
    case SR_INT32_T:
        gpb_value->int32_val = value->data.int32_val;
        gpb_value->has_int32_val = true;
        break;
    case SR_INT64_T:
        gpb_value->int64_val = value->data.int64_val;
        gpb_value->has_int64_val = true;
        break;
    case SR_STRING_T:
        if (value->_sr_mem) {
            gpb_value->string_val = value->data.string_val;
        } else if (NULL != value->data.string_val) {
            gpb_value->string_val = strdup(value->data.string_val);
            CHECK_NULL_NOMEM_RETURN(gpb_value->string_val);
        }
        return SR_ERR_OK;
    case SR_UINT8_T:
        gpb_value->uint8_val = value->data.uint8_val;
        gpb_value->has_uint8_val = true;
        break;
    case SR_UINT16_T:
        gpb_value->uint16_val = value->data.uint16_val;
        gpb_value->has_uint16_val = true;
        break;
    case SR_UINT32_T:
        gpb_value->uint32_val = value->data.uint32_val;
        gpb_value->has_uint32_val = true;
        break;
    case SR_UINT64_T:
        gpb_value->uint64_val = value->data.uint64_val;
        gpb_value->has_uint64_val = true;
        break;
    case SR_ANYXML_T:
        if (value->_sr_mem) {
            gpb_value->anyxml_val = value->data.anyxml_val;
        } else if (NULL != value->data.anyxml_val) {
            gpb_value->anyxml_val = strdup(value->data.anyxml_val);
            CHECK_NULL_NOMEM_RETURN(gpb_value->anyxml_val);
        }
        return SR_ERR_OK;
    case SR_ANYDATA_T:
        if (value->_sr_mem) {
            gpb_value->anydata_val = value->data.anydata_val;
        } else if (NULL != value->data.anydata_val) {
            gpb_value->anydata_val = strdup(value->data.anydata_val);
            CHECK_NULL_NOMEM_RETURN(gpb_value->anydata_val);
        }
        return SR_ERR_OK;
    default:
        SR_LOG_ERR("Conversion of value type not supported '%s'", value->xpath);
        return SR_ERR_INTERNAL;
    }

    return SR_ERR_OK;
}

int
sr_dup_val_t_to_gpb(const sr_val_t *value, Sr__Value **gpb_value){
    CHECK_NULL_ARG2(value, gpb_value);
    int rc = SR_ERR_OK;
    Sr__Value *gpb;
    sr_mem_snapshot_t snapshot = { 0, };

    if (value->_sr_mem) {
        sr_mem_snapshot(value->_sr_mem, &snapshot);
    }

    gpb = sr_calloc(value->_sr_mem, 1, sizeof(*gpb));
    CHECK_NULL_NOMEM_RETURN(gpb);

    sr__value__init(gpb);

    rc = sr_set_val_t_type_in_gpb(value, gpb);
    CHECK_RC_LOG_GOTO(rc, cleanup, "Setting type in gpb failed for xpath '%s'", value->xpath);

    rc = sr_set_val_t_value_in_gpb(value, gpb);
    CHECK_RC_LOG_GOTO(rc, cleanup, "Setting value in gpb failed for xpath '%s'", value->xpath);

    *gpb_value = gpb;
    return rc;

cleanup:
    if (value->_sr_mem) {
        sr_mem_restore(&snapshot);
    } else {
        sr__value__free_unpacked(gpb, NULL);
    }
    return rc;
}


static int
sr_set_gpb_type_in_val_t(const Sr__Value *gpb_value, sr_val_t *value){
    CHECK_NULL_ARG2(value, gpb_value);
    int rc = SR_ERR_OK;
    switch (gpb_value->type) {
    case SR__VALUE__TYPES__LIST:
        value->type = SR_LIST_T;
        break;
    case SR__VALUE__TYPES__CONTAINER:
        value->type = SR_CONTAINER_T;
        break;
    case SR__VALUE__TYPES__CONTAINER_PRESENCE:
        value->type = SR_CONTAINER_PRESENCE_T;
        break;
    case SR__VALUE__TYPES__LEAF_EMPTY:
        value->type = SR_LEAF_EMPTY_T;
        break;
    case SR__VALUE__TYPES__BINARY:
        value->type = SR_BINARY_T;
        break;
    case SR__VALUE__TYPES__BITS:
        value->type = SR_BITS_T;
        break;
    case SR__VALUE__TYPES__BOOL:
        value->type = SR_BOOL_T;
        break;
    case SR__VALUE__TYPES__DECIMAL64:
        value->type = SR_DECIMAL64_T;
        break;
    case SR__VALUE__TYPES__ENUM:
        value->type = SR_ENUM_T;
        break;
    case SR__VALUE__TYPES__IDENTITYREF:
        value->type = SR_IDENTITYREF_T;
        break;
    case SR__VALUE__TYPES__INSTANCEID:
        value->type = SR_INSTANCEID_T;
        break;
    case SR__VALUE__TYPES__INT8:
        value->type = SR_INT8_T;
        break;
    case SR__VALUE__TYPES__INT16:
        value->type = SR_INT16_T;
        break;
    case SR__VALUE__TYPES__INT32:
        value->type = SR_INT32_T;
        break;
    case SR__VALUE__TYPES__INT64:
        value->type = SR_INT64_T;
        break;
    case SR__VALUE__TYPES__STRING:
        value->type = SR_STRING_T;
        break;
    case SR__VALUE__TYPES__UINT8:
        value->type = SR_UINT8_T;
        break;
    case SR__VALUE__TYPES__UINT16:
        value->type = SR_UINT16_T;
        break;
    case SR__VALUE__TYPES__UINT32:
        value->type = SR_UINT32_T;
        break;
    case SR__VALUE__TYPES__UINT64:
        value->type = SR_UINT64_T;
        break;
    case SR__VALUE__TYPES__ANYXML:
        value->type = SR_ANYXML_T;
        break;
    case SR__VALUE__TYPES__ANYDATA:
        value->type = SR_ANYDATA_T;
        break;
    default:
        SR_LOG_ERR_MSG("Type can not be mapped to sr_val_t");
        return SR_ERR_INTERNAL;
    }

    return rc;
}

/**
 * @brief Copies data GPB message to sr_val_t. Makes shallow copy if the value
 * (and GPB message) is allocated inside Sysrepo memory context, otherwise a deep copy.
 */
static int
sr_set_gpb_value_in_val_t(const Sr__Value *gpb_value, sr_val_t *value){
    CHECK_NULL_ARG3(value, gpb_value, gpb_value->xpath);

    if (value->_sr_mem) {
        value->xpath = gpb_value->xpath;
    } else {
        value->xpath = strdup(gpb_value->xpath);
        CHECK_NULL_NOMEM_RETURN(value->xpath);
    }
    value->dflt = gpb_value->dflt;

    switch (gpb_value->type) {
    case SR__VALUE__TYPES__LIST:
    case SR__VALUE__TYPES__CONTAINER:
    case SR__VALUE__TYPES__CONTAINER_PRESENCE:
    case SR__VALUE__TYPES__LEAF_EMPTY:
        return SR_ERR_OK;
    case SR__VALUE__TYPES__BINARY:
        if (value->_sr_mem) {
            value->data.binary_val = gpb_value->binary_val;
        } else {
            value->data.binary_val = strdup(gpb_value->binary_val);
            CHECK_NULL_NOMEM_RETURN(gpb_value->binary_val);
        }
        return SR_ERR_OK;
    case SR__VALUE__TYPES__BITS:
        if (value->_sr_mem) {
            value->data.bits_val = gpb_value->bits_val;
        } else {
            value->data.bits_val = strdup(gpb_value->bits_val);
            CHECK_NULL_NOMEM_RETURN(gpb_value->bits_val);
        }
        return SR_ERR_OK;
    case SR__VALUE__TYPES__BOOL:
        value->data.bool_val = gpb_value->bool_val;
        return SR_ERR_OK;
    case SR__VALUE__TYPES__DECIMAL64:
        value->data.decimal64_val = gpb_value->decimal64_val;
        return SR_ERR_OK;
    case SR__VALUE__TYPES__ENUM:
        if (value->_sr_mem) {
            value->data.enum_val = gpb_value->enum_val;
        } else {
            value->data.enum_val = strdup(gpb_value->enum_val);
            CHECK_NULL_NOMEM_RETURN(gpb_value->enum_val);
        }
        return SR_ERR_OK;
    case SR__VALUE__TYPES__IDENTITYREF:
        if (value->_sr_mem) {
            value->data.identityref_val = gpb_value->identityref_val;
        } else {
            value->data.identityref_val = strdup(gpb_value->identityref_val);
            CHECK_NULL_NOMEM_RETURN(gpb_value->identityref_val);
        }
        return SR_ERR_OK;
    case SR__VALUE__TYPES__INSTANCEID:
        if (value->_sr_mem) {
            value->data.instanceid_val = gpb_value->instanceid_val;
        } else {
            value->data.instanceid_val = strdup(gpb_value->instanceid_val);
            CHECK_NULL_NOMEM_RETURN(gpb_value->instanceid_val);
        }
        return SR_ERR_OK;
    case SR__VALUE__TYPES__INT8:
        value->data.int8_val = gpb_value->int8_val;
        return SR_ERR_OK;
    case SR__VALUE__TYPES__INT16:
        value->data.int16_val = gpb_value->int16_val;
        return SR_ERR_OK;
    case SR__VALUE__TYPES__INT32:
        value->data.int32_val = gpb_value->int32_val;
        return SR_ERR_OK;
    case SR__VALUE__TYPES__INT64:
        value->data.int64_val = gpb_value->int64_val;
        return SR_ERR_OK;
    case SR__VALUE__TYPES__STRING:
        if (value->_sr_mem) {
            value->data.string_val = gpb_value->string_val;
        } else {
            value->data.string_val = strdup(gpb_value->string_val);
            CHECK_NULL_NOMEM_RETURN(gpb_value->string_val);
        }
        return SR_ERR_OK;
    case SR__VALUE__TYPES__UINT8:
        value->data.uint8_val = gpb_value->uint8_val;
        return SR_ERR_OK;
    case SR__VALUE__TYPES__UINT16:
        value->data.uint16_val = gpb_value->uint16_val;
        return SR_ERR_OK;
    case SR__VALUE__TYPES__UINT32:
        value->data.uint32_val = gpb_value->uint32_val;
        return SR_ERR_OK;
    case SR__VALUE__TYPES__UINT64:
        value->data.uint64_val = gpb_value->uint64_val;
        return SR_ERR_OK;
    case SR__VALUE__TYPES__ANYXML:
        if (value->_sr_mem) {
            value->data.anyxml_val = gpb_value->anyxml_val;
        } else {
            value->data.anyxml_val = strdup(gpb_value->anyxml_val);
            CHECK_NULL_NOMEM_RETURN(gpb_value->anyxml_val);
        }
        return SR_ERR_OK;
    case SR__VALUE__TYPES__ANYDATA:
        if (value->_sr_mem) {
            value->data.anydata_val = gpb_value->anydata_val;
        } else {
            value->data.anydata_val = strdup(gpb_value->anydata_val);
            CHECK_NULL_NOMEM_RETURN(gpb_value->anydata_val);
        }
        return SR_ERR_OK;
    default:
        SR_LOG_ERR_MSG("Copy of value failed");
        return SR_ERR_INTERNAL;
    }
    return SR_ERR_OK;
}

int
sr_copy_gpb_to_val_t(const Sr__Value *gpb_value, sr_val_t *value)
{
    CHECK_NULL_ARG2(gpb_value, value);
    int rc = SR_ERR_INTERNAL;

    rc = sr_set_gpb_type_in_val_t(gpb_value, value);
    if (SR_ERR_OK != rc) {
        SR_LOG_ERR_MSG("Setting type in for sr_value_t failed");
        return rc;
    }

    rc = sr_set_gpb_value_in_val_t(gpb_value, value);
    if (SR_ERR_OK != rc) {
        SR_LOG_ERR_MSG("Setting value in for sr_value_t failed");
        return rc;
    }

    return rc;
}

int
sr_dup_gpb_to_val_t(sr_mem_ctx_t *sr_mem, const Sr__Value *gpb_value, sr_val_t **value)
{
    CHECK_NULL_ARG2(gpb_value, value);
    sr_val_t *val = NULL;
    sr_mem_snapshot_t snapshot = { 0, };
    int rc = SR_ERR_OK;

    if (sr_mem) {
        sr_mem_snapshot(sr_mem, &snapshot);
    }

    val = sr_calloc(sr_mem, 1, sizeof(*val));
    CHECK_NULL_NOMEM_RETURN(val);
    val->_sr_mem = sr_mem;

    rc = sr_copy_gpb_to_val_t(gpb_value, val);
    if (SR_ERR_OK != rc) {
        if (sr_mem) {
            sr_mem_restore(&snapshot);
        } else {
            free(val);
        }
        return rc;
    }

    if (sr_mem) {
        ++sr_mem->obj_count;
    }
    *value = val;
    return rc;
}

int
sr_values_sr_to_gpb(const sr_val_t *sr_values, const size_t sr_value_cnt, Sr__Value ***gpb_values_p, size_t *gpb_value_cnt_p)
{
    Sr__Value **gpb_values = NULL;
    sr_mem_ctx_t *sr_mem = NULL;
    sr_mem_snapshot_t snapshot = { 0, };
    int rc = SR_ERR_OK;

    CHECK_NULL_ARG2(gpb_values_p, gpb_value_cnt_p);

    if ((NULL != sr_values) && (sr_value_cnt > 0)) {
        sr_mem = sr_values[0]._sr_mem;
        if (NULL != sr_mem) {
            sr_mem_snapshot(sr_mem, &snapshot);
        }
        gpb_values = sr_calloc(sr_mem, sr_value_cnt, sizeof(*gpb_values));
        CHECK_NULL_NOMEM_RETURN(gpb_values);

        for (size_t i = 0; i < sr_value_cnt; i++) {
            rc = sr_dup_val_t_to_gpb(&sr_values[i], &gpb_values[i]);
            CHECK_RC_MSG_GOTO(rc, cleanup, "Unable to duplicate sr_val_t to GPB.");
        }
    }

    *gpb_values_p = gpb_values;
    *gpb_value_cnt_p = sr_value_cnt;

    return SR_ERR_OK;

cleanup:
    if (NULL == sr_mem) {
        for (size_t i = 0; i < sr_value_cnt; i++) {
            if (NULL != gpb_values[i]) {
                sr__value__free_unpacked(gpb_values[i], NULL);
            }
        }
        free(gpb_values);
    } else {
        sr_mem_restore(&snapshot);
    }
    return rc;
}

int
sr_values_gpb_to_sr(sr_mem_ctx_t *sr_mem, Sr__Value **gpb_values, size_t gpb_value_cnt, sr_val_t **sr_values_p,
        size_t *sr_value_cnt_p)
{
    sr_val_t *sr_values = NULL;
    sr_mem_snapshot_t snapshot = { 0, };
    int rc = SR_ERR_OK;

    CHECK_NULL_ARG2(sr_values_p, sr_value_cnt_p);

    if ((NULL != gpb_values) && (gpb_value_cnt > 0)) {
        if (sr_mem) {
            sr_mem_snapshot(sr_mem, &snapshot);
        }
        sr_values = sr_calloc(sr_mem, gpb_value_cnt, sizeof(*sr_values));
        CHECK_NULL_NOMEM_RETURN(sr_values);
        if (sr_mem) {
            for (size_t i = 0; i < gpb_value_cnt; i++) {
                sr_values[i]._sr_mem = sr_mem;
            }
        }

        for (size_t i = 0; i < gpb_value_cnt; i++) {
            rc = sr_copy_gpb_to_val_t(gpb_values[i], &sr_values[i]);
            CHECK_RC_MSG_GOTO(rc, cleanup, "Unable to duplicate GPB value to sr_val_t.");
        }
    }

    if (sr_mem && sr_values) {
        ++sr_mem->obj_count;
    }
    *sr_values_p = sr_values;
    *sr_value_cnt_p = gpb_value_cnt;

    return SR_ERR_OK;

cleanup:
    if (sr_mem) {
        sr_mem_restore(&snapshot);
    } else {
        sr_free_values(sr_values, gpb_value_cnt);
    }
    return rc;
}

int
sr_dup_tree_to_gpb(const sr_node_t *sr_tree, Sr__Node **gpb_tree)
{
    CHECK_NULL_ARG2(sr_tree, gpb_tree);
    int rc = SR_ERR_OK, i = 0;
    int children_cnt = 0;
    sr_node_t *sr_subtree = NULL;
    Sr__Node *gpb;
    sr_mem_snapshot_t snapshot = { 0, };

    if (sr_tree->_sr_mem) {
        sr_mem_snapshot(sr_tree->_sr_mem, &snapshot);
    }

    gpb = sr_calloc(sr_tree->_sr_mem, 1, sizeof(*gpb));
    CHECK_NULL_NOMEM_RETURN(gpb);
    sr__node__init(gpb);
    gpb->value = sr_calloc(sr_tree->_sr_mem, 1, sizeof(*gpb->value));
    CHECK_NULL_NOMEM_GOTO(gpb->value, rc, cleanup);
    sr__value__init(gpb->value);
    gpb->n_children = 0;

    /* set members which are common with sr_val_t */
    rc = sr_set_val_t_type_in_gpb((sr_val_t *)sr_tree, gpb->value);
    CHECK_RC_LOG_GOTO(rc, cleanup, "Setting value type in gpb tree failed for node '%s'", sr_tree->name);

    rc = sr_set_val_t_value_in_gpb((sr_val_t *)sr_tree, gpb->value);
    CHECK_RC_LOG_GOTO(rc, cleanup, "Setting value in gpb tree failed for node '%s'", sr_tree->name);

    /* module_name */
    if (NULL != sr_tree->module_name) {
        if (NULL != sr_tree->_sr_mem) {
            gpb->module_name = sr_tree->module_name;
        } else {
            gpb->module_name = strdup(sr_tree->module_name);
            CHECK_NULL_NOMEM_GOTO(gpb->module_name, rc, cleanup);
        }
    }

    /* recursively duplicate children */
    sr_subtree = sr_tree->first_child;
    while (sr_subtree) {
        ++children_cnt;
        sr_subtree = sr_subtree->next;
    }
    if (0 < children_cnt) {
        gpb->children = sr_calloc(sr_tree->_sr_mem, children_cnt, sizeof(*gpb->children));
        CHECK_NULL_NOMEM_GOTO(gpb->children, rc, cleanup);
        sr_subtree = sr_tree->first_child;
        i = 0;
        while (sr_subtree) {
            rc = sr_dup_tree_to_gpb(sr_subtree, gpb->children + i);
            if (SR_ERR_OK != rc) {
                goto cleanup;
            }
            ++i;
            ++gpb->n_children;
            sr_subtree = sr_subtree->next;
        }
    }

    *gpb_tree = gpb;
    return rc;

cleanup:
    if (sr_tree->_sr_mem) {
        sr_mem_restore(&snapshot);
    } else {
        sr__node__free_unpacked(gpb, NULL);
    }
    return rc;
}

int
sr_dup_gpb_to_tree(sr_mem_ctx_t *sr_mem, const Sr__Node *gpb_tree, sr_node_t **sr_tree)
{
    CHECK_NULL_ARG2(gpb_tree, sr_tree);
    sr_node_t *tree = NULL;
    sr_mem_snapshot_t snapshot = { 0, };
    int rc = SR_ERR_OK;

    if (sr_mem) {
        sr_mem_snapshot(sr_mem, &snapshot);
    }

    tree = sr_calloc(sr_mem, 1, sizeof *tree);
    CHECK_NULL_NOMEM_RETURN(tree);
    tree->_sr_mem = sr_mem;

    rc = sr_copy_gpb_to_tree(gpb_tree, tree);
    if (SR_ERR_OK != rc) {
        if (sr_mem) {
            sr_mem_restore(&snapshot);
        } else {
            sr_free_tree(tree);
        }
        return rc;
    }

    if (sr_mem) {
        ++sr_mem->obj_count;
    }
    *sr_tree = tree;
    return rc;
}

int
sr_copy_gpb_to_tree(const Sr__Node *gpb_tree, sr_node_t *sr_tree)
{
    CHECK_NULL_ARG2(gpb_tree, sr_tree);
    sr_node_t *sr_subtree = NULL;
    int rc = SR_ERR_INTERNAL;

    /* members common with sr_val_t */
    rc = sr_set_gpb_type_in_val_t(gpb_tree->value, (sr_val_t *)sr_tree);
    if (SR_ERR_OK != rc) {
        SR_LOG_ERR_MSG("Setting value type in for sr_value_t failed");
        return rc;
    }

    rc = sr_set_gpb_value_in_val_t(gpb_tree->value, (sr_val_t *)sr_tree);
    if (SR_ERR_OK != rc) {
        SR_LOG_ERR_MSG("Setting value in for sr_value_t failed");
        return rc;
    }

    /* module_name */
    if (NULL != gpb_tree->module_name && strlen(gpb_tree->module_name)) {
        if (NULL != sr_tree->_sr_mem) {
            sr_tree->module_name = gpb_tree->module_name;
        } else {
            sr_tree->module_name = strdup(gpb_tree->module_name);
            CHECK_NULL_NOMEM_GOTO(sr_tree->module_name, rc, cleanup);
        }
    } else {
        sr_tree->module_name = NULL;
    }

    /* recursively copy children */
    sr_tree->first_child = NULL;
    sr_tree->last_child = NULL;
    if (gpb_tree->n_children) {
        for (size_t i = 0; i < gpb_tree->n_children; ++i) {
            rc = sr_node_add_child(sr_tree, NULL, NULL, &sr_subtree);
            if (SR_ERR_OK != rc) {
                goto cleanup;
            }
            rc = sr_copy_gpb_to_tree(gpb_tree->children[i], sr_subtree);
            if (SR_ERR_OK != rc) {
                goto cleanup;
            }
        }
    }

cleanup:
    if (SR_ERR_OK != rc) {
        sr_free_tree_content(sr_tree);
    }
    return rc;
}

int
sr_trees_sr_to_gpb(const sr_node_t *sr_trees, const size_t sr_tree_cnt, Sr__Node ***gpb_trees_p, size_t *gpb_tree_cnt_p)
{
    Sr__Node **gpb_trees = NULL;
    sr_mem_ctx_t *sr_mem = NULL;
    sr_mem_snapshot_t snapshot = { 0, };
    int rc = SR_ERR_OK;

    CHECK_NULL_ARG2(gpb_trees_p, gpb_tree_cnt_p);

    if ((NULL != sr_trees) && (sr_tree_cnt > 0)) {
        sr_mem = sr_trees[0]._sr_mem;
        if (NULL != sr_mem) {
            sr_mem_snapshot(sr_mem, &snapshot);
        }
        gpb_trees = sr_calloc(sr_mem, sr_tree_cnt, sizeof(*gpb_trees));
        CHECK_NULL_NOMEM_RETURN(gpb_trees);

        for (size_t i = 0; i < sr_tree_cnt; i++) {
            rc = sr_dup_tree_to_gpb(&sr_trees[i], &gpb_trees[i]);
            CHECK_RC_MSG_GOTO(rc, cleanup, "Unable to duplicate sysrepo tree to GPB.");
        }
    }

    *gpb_trees_p = gpb_trees;
    *gpb_tree_cnt_p = sr_tree_cnt;

    return SR_ERR_OK;

cleanup:
    if (NULL == sr_mem) {
        for (size_t i = 0; i < sr_tree_cnt; i++) {
            sr__node__free_unpacked(gpb_trees[i], NULL);
        }
        free(gpb_trees);
    } else {
        sr_mem_restore(&snapshot);
    }
    return rc;
}

int
sr_trees_gpb_to_sr(sr_mem_ctx_t *sr_mem, Sr__Node **gpb_trees, size_t gpb_tree_cnt, sr_node_t **sr_trees_p, size_t *sr_tree_cnt_p)
{
    sr_node_t *sr_trees = NULL;
    sr_mem_snapshot_t snapshot = { 0, };
    int rc = SR_ERR_OK;

    CHECK_NULL_ARG2(sr_trees_p, sr_tree_cnt_p);

    if ((NULL != gpb_trees) && (gpb_tree_cnt > 0)) {
        if (sr_mem) {
            sr_mem_snapshot(sr_mem, &snapshot);
        }
        sr_trees = sr_calloc(sr_mem, gpb_tree_cnt, sizeof(*sr_trees));
        CHECK_NULL_NOMEM_RETURN(sr_trees);
        if (sr_mem) {
            for (size_t i = 0; i < gpb_tree_cnt; i++) {
                sr_trees[i]._sr_mem = sr_mem;
            }
        }

        for (size_t i = 0; i < gpb_tree_cnt; i++) {
            rc = sr_copy_gpb_to_tree(gpb_trees[i], &sr_trees[i]);
            CHECK_RC_MSG_GOTO(rc, cleanup, "Unable to duplicate GPB tree to sysrepo tree.");
        }
    }

    if (sr_mem && sr_trees) {
        ++sr_mem->obj_count;
    }
    *sr_trees_p = sr_trees;
    *sr_tree_cnt_p = gpb_tree_cnt;

    return SR_ERR_OK;

cleanup:
    if (sr_mem) {
        sr_mem_restore(&snapshot);
    } else {
        sr_free_trees(sr_trees, gpb_tree_cnt);
    }
    return rc;
}

int
sr_changes_sr_to_gpb(sr_list_t *sr_changes, sr_mem_ctx_t *sr_mem, Sr__Change ***gpb_changes_p, size_t *gpb_count)
{
    Sr__Change **gpb_changes = NULL;
    sr_mem_snapshot_t snapshot = { 0, };
    sr_val_t *value_dup = NULL;
    int rc = SR_ERR_OK;

    CHECK_NULL_ARG2(gpb_changes_p, gpb_count);

    if ((NULL != sr_changes) && (sr_changes->count > 0)) {
        if (sr_mem) {
            sr_mem_snapshot(sr_mem, &snapshot);
        }
        gpb_changes = sr_calloc(sr_mem, sr_changes->count, sizeof(*gpb_changes));
        CHECK_NULL_NOMEM_RETURN(gpb_changes);

        for (size_t i = 0; i < sr_changes->count; i++) {
            gpb_changes[i] = sr_calloc(sr_mem, 1, sizeof(**gpb_changes));
            CHECK_NULL_NOMEM_GOTO(gpb_changes[i], rc, cleanup);
            sr__change__init(gpb_changes[i]);
            sr_change_t *ch = sr_changes->data[i];
            if (NULL != ch->new_value) {
                if (sr_mem) {
                    rc = sr_dup_val_ctx(ch->new_value, sr_mem, &value_dup);
                    CHECK_RC_MSG_GOTO(rc, cleanup, "Unable to duplicate sr_val_t.");
                    --sr_mem->obj_count; /* do not treat value_dup as an object on its own */
                } else {
                    value_dup = ch->new_value;
                }
                rc = sr_dup_val_t_to_gpb(value_dup, &gpb_changes[i]->new_value);
                CHECK_RC_MSG_GOTO(rc, cleanup, "Unable to duplicate sr_val_t to GPB.");
            }
            if (NULL != ch->old_value) {
                if (sr_mem) {
                    rc = sr_dup_val_ctx(ch->old_value, sr_mem, &value_dup);
                    CHECK_RC_MSG_GOTO(rc, cleanup, "Unable to duplicate sr_val_t.");
                    --sr_mem->obj_count; /* do not treat value_dup as an object on its own */
                } else {
                    value_dup = ch->old_value;
                }
                rc = sr_dup_val_t_to_gpb(value_dup, &gpb_changes[i]->old_value);
                CHECK_RC_MSG_GOTO(rc, cleanup, "Unable to duplicate sr_val_t to GPB.");
            }
            gpb_changes[i]->changeoperation = sr_change_op_sr_to_gpb(ch->oper);
        }
    }

    *gpb_changes_p = gpb_changes;
    *gpb_count = NULL != sr_changes ? sr_changes->count : 0;

    return SR_ERR_OK;

cleanup:
    if (sr_mem) {
        sr_mem_restore(&snapshot);
    } else {
        for (size_t i = 0; i < sr_changes->count; i++) {
            sr__change__free_unpacked(gpb_changes[i], NULL);
        }
        free(gpb_changes);
    }
    return rc;
}

Sr__DataStore
sr_datastore_sr_to_gpb(const sr_datastore_t sr_ds)
{
    switch (sr_ds) {
        case SR_DS_CANDIDATE:
            return SR__DATA_STORE__CANDIDATE;
        case SR_DS_RUNNING:
            return SR__DATA_STORE__RUNNING;
        case SR_DS_STARTUP:
            /* fall through */
        default:
            return SR__DATA_STORE__STARTUP;
    }
}

sr_datastore_t
sr_datastore_gpb_to_sr(Sr__DataStore gpb_ds)
{
    switch (gpb_ds) {
        case SR__DATA_STORE__CANDIDATE:
            return SR_DS_CANDIDATE;
        case SR__DATA_STORE__RUNNING:
            return SR_DS_RUNNING;
        case SR__DATA_STORE__STARTUP:
            /* fall through */
        default:
            return SR_DS_STARTUP;
    }
}

sr_change_oper_t
sr_change_op_gpb_to_sr(Sr__ChangeOperation gpb_ch)
{
    switch (gpb_ch) {
    case SR__CHANGE_OPERATION__CREATED:
        return SR_OP_CREATED;
    case SR__CHANGE_OPERATION__DELETED:
        return SR_OP_DELETED;
    case SR__CHANGE_OPERATION__MOVED:
        return SR_OP_MOVED;
    case SR__CHANGE_OPERATION__MODIFIED:
    default:
        /* fall through */
        return SR_OP_MODIFIED;
    }
}

Sr__ChangeOperation
sr_change_op_sr_to_gpb(sr_change_oper_t sr_ch)
{
    switch (sr_ch) {
    case SR_OP_CREATED:
        return SR__CHANGE_OPERATION__CREATED;
    case SR_OP_DELETED:
        return SR__CHANGE_OPERATION__DELETED;
    case SR_OP_MOVED:
        return SR__CHANGE_OPERATION__MOVED;
    case SR_OP_MODIFIED:
    default:
        /* fall through */
        return SR__CHANGE_OPERATION__MODIFIED;
    }
}

Sr__MoveItemReq__MovePosition
sr_move_position_sr_to_gpb(sr_move_position_t sr_position)
{
    switch (sr_position) {
        case SR_MOVE_BEFORE:
            return SR__MOVE_ITEM_REQ__MOVE_POSITION__BEFORE;
        case SR_MOVE_AFTER:
            return SR__MOVE_ITEM_REQ__MOVE_POSITION__AFTER;
        case SR_MOVE_FIRST:
            return SR__MOVE_ITEM_REQ__MOVE_POSITION__FIRST;
        case SR_MOVE_LAST:
            /* fall through */
        default:
            return SR__MOVE_ITEM_REQ__MOVE_POSITION__LAST;
    }
}

sr_move_position_t
sr_move_direction_gpb_to_sr(Sr__MoveItemReq__MovePosition gpb_position)
{
    switch (gpb_position) {
        case SR__MOVE_ITEM_REQ__MOVE_POSITION__BEFORE:
            return SR_MOVE_BEFORE;
        case SR__MOVE_ITEM_REQ__MOVE_POSITION__AFTER:
            return SR_MOVE_AFTER;
        case SR__MOVE_ITEM_REQ__MOVE_POSITION__FIRST:
            return SR_MOVE_FIRST;
        case SR__MOVE_ITEM_REQ__MOVE_POSITION__LAST:
            /* fall through */
        default:
            return SR_MOVE_LAST;
    }
}

char *
sr_subscription_type_gpb_to_str(Sr__SubscriptionType type)
{
    switch (type) {
        case SR__SUBSCRIPTION_TYPE__MODULE_INSTALL_SUBS:
            return "module-install";
        case SR__SUBSCRIPTION_TYPE__FEATURE_ENABLE_SUBS:
            return "feature-enable";
        case SR__SUBSCRIPTION_TYPE__MODULE_CHANGE_SUBS:
            return "module-change";
        case SR__SUBSCRIPTION_TYPE__SUBTREE_CHANGE_SUBS:
            return "subtree-change";
        case SR__SUBSCRIPTION_TYPE__DP_GET_ITEMS_SUBS:
            return "dp-get-items";
        case SR__SUBSCRIPTION_TYPE__RPC_SUBS:
            return "rpc";
        case SR__SUBSCRIPTION_TYPE__ACTION_SUBS:
            return "action";
        case SR__SUBSCRIPTION_TYPE__EVENT_NOTIF_SUBS:
            return "event-notification";
        case SR__SUBSCRIPTION_TYPE__HELLO_SUBS:
            return "hello";
        case SR__SUBSCRIPTION_TYPE__COMMIT_END_SUBS:
            return "commit-end";
        default:
            return "unknown";
    }
}

Sr__SubscriptionType
sr_subsciption_type_str_to_gpb(const char *type_name)
{
    if (0 == strcmp(type_name, "module-install")) {
        return SR__SUBSCRIPTION_TYPE__MODULE_INSTALL_SUBS;
    }
    if (0 == strcmp(type_name, "feature-enable")) {
        return SR__SUBSCRIPTION_TYPE__FEATURE_ENABLE_SUBS;
    }
    if (0 == strcmp(type_name, "module-change")) {
        return SR__SUBSCRIPTION_TYPE__MODULE_CHANGE_SUBS;
    }
    if (0 == strcmp(type_name, "subtree-change")) {
        return SR__SUBSCRIPTION_TYPE__SUBTREE_CHANGE_SUBS;
    }
    if (0 == strcmp(type_name, "dp-get-items")) {
        return SR__SUBSCRIPTION_TYPE__DP_GET_ITEMS_SUBS;
    }
    if (0 == strcmp(type_name, "rpc")) {
        return SR__SUBSCRIPTION_TYPE__RPC_SUBS;
    }
    if (0 == strcmp(type_name, "action")) {
        return SR__SUBSCRIPTION_TYPE__ACTION_SUBS;
    }
    if (0 == strcmp(type_name, "hello")) {
        return SR__SUBSCRIPTION_TYPE__HELLO_SUBS;
    }
    if (0 == strcmp(type_name, "commit-end")) {
        return SR__SUBSCRIPTION_TYPE__COMMIT_END_SUBS;
    }
    if (0 == strcmp(type_name, "event-notification")) {
        return SR__SUBSCRIPTION_TYPE__EVENT_NOTIF_SUBS;
    }
    SR_LOG_ERR("Unknown type %s can not be converted", type_name);
    return _SR__SUBSCRIPTION_TYPE_IS_INT_SIZE;
}

char *
sr_notification_event_gpb_to_str(Sr__NotificationEvent event)
{
    switch (event) {
        case SR__NOTIFICATION_EVENT__VERIFY_EV:
            return "verify";
        case SR__NOTIFICATION_EVENT__APPLY_EV:
            return "apply";
        case SR__NOTIFICATION_EVENT__ABORT_EV:
            return "abort";
        case SR__NOTIFICATION_EVENT__ENABLED_EV:
            return "enabled";
        default:
            return "unknown";
    }
}

char *
sr_notification_event_sr_to_str(sr_notif_event_t event)
{
    switch (event) {
        case SR_EV_VERIFY:
            return "verify";
        case SR_EV_APPLY:
            return "apply";
        case SR_EV_ABORT:
            return "abort";
        case SR_EV_ENABLED:
            return "enabled";
        default:
            return "unknown";
    }
}

Sr__NotificationEvent
sr_notification_event_sr_to_gpb(sr_notif_event_t event)
{
    switch (event) {
    case SR_EV_VERIFY:
        return SR__NOTIFICATION_EVENT__VERIFY_EV;
    case SR_EV_APPLY:
        return SR__NOTIFICATION_EVENT__APPLY_EV;
    case SR_EV_ENABLED:
        return SR__NOTIFICATION_EVENT__ENABLED_EV;
    case SR_EV_ABORT:
    default:
        return SR__NOTIFICATION_EVENT__ABORT_EV;
    }
}

Sr__NotificationEvent
sr_notification_event_str_to_gpb(const char *event_name)
{
    if (0 == strcmp(event_name, "verify")) {
        return SR__NOTIFICATION_EVENT__VERIFY_EV;
    }
    if (0 == strcmp(event_name, "apply")) {
        return SR__NOTIFICATION_EVENT__APPLY_EV;
    }
    if (0 == strcmp(event_name, "abort")) {
        return SR__NOTIFICATION_EVENT__ABORT_EV;
    }
    if (0 == strcmp(event_name, "enabled")) {
        return SR__NOTIFICATION_EVENT__ENABLED_EV;
    }
    return _SR__NOTIFICATION_EVENT_IS_INT_SIZE;
}

sr_notif_event_t
sr_notification_event_gpb_to_sr(Sr__NotificationEvent event)
{
    switch (event) {
        case SR__NOTIFICATION_EVENT__VERIFY_EV:
            return SR_EV_VERIFY;
        case SR__NOTIFICATION_EVENT__APPLY_EV:
            return SR_EV_APPLY;
        case SR__NOTIFICATION_EVENT__ENABLED_EV:
            return SR_EV_ENABLED;
        default:
            return SR_EV_ABORT;
    }
}

sr_ev_notif_type_t
sr_ev_notification_type_gpb_to_sr(Sr__EventNotifReq__NotifType ev_notif_type)
{
    switch (ev_notif_type) {
        case SR__EVENT_NOTIF_REQ__NOTIF_TYPE__REALTIME:
            return SR_EV_NOTIF_T_REALTIME;
        case SR__EVENT_NOTIF_REQ__NOTIF_TYPE__REPLAY:
            return SR_EV_NOTIF_T_REPLAY;
        case SR__EVENT_NOTIF_REQ__NOTIF_TYPE__REPLAY_COMPLETE:
            return SR_EV_NOTIF_T_REPLAY_COMPLETE;
        case SR__EVENT_NOTIF_REQ__NOTIF_TYPE__REPLAY_STOP:
            return SR_EV_NOTIF_T_REPLAY_STOP;
        default:
            return SR_EV_NOTIF_T_REALTIME;
    }
}

Sr__ApiVariant
sr_api_variant_sr_to_gpb(sr_api_variant_t api_variant)
{
    switch (api_variant) {
        case SR_API_VALUES:
            return SR__API_VARIANT__VALUES;
        case SR_API_TREES:
            return SR__API_VARIANT__TREES;
        default:
            return SR__API_VARIANT__VALUES;
    }
}

sr_api_variant_t
sr_api_variant_gpb_to_sr(Sr__ApiVariant api_variant_gpb)
{
    switch (api_variant_gpb) {
        case SR__API_VARIANT__VALUES:
            return SR_API_VALUES;
        case SR__API_VARIANT__TREES:
            return SR_API_TREES;
        default:
            return SR_API_VALUES;
    }
}

char *
sr_module_state_sr_to_str(sr_module_state_t state)
{
    switch (state) {
        case SR_MS_UNINSTALLED:
            return "uninstalled";
        case SR_MS_IMPORTED:
            return "imported";
        case SR_MS_IMPLEMENTED:
            return "implemented";
        default:
            return "unknown";
    }
}

Sr__ModuleState
sr_module_state_sr_to_gpb(sr_module_state_t state)
{
    switch (state) {
    case SR_MS_UNINSTALLED:
        return SR__MODULE_STATE__UNINSTALLED;
    case SR_MS_IMPORTED:
        return SR__MODULE_STATE__IMPORTED;
    case SR_MS_IMPLEMENTED:
        return SR__MODULE_STATE__IMPLEMENTED;
    default:
        return SR__MODULE_STATE__UNINSTALLED;
    }
}

sr_module_state_t
sr_module_state_gpb_to_sr(Sr__ModuleState state)
{
    switch (state) {
        case SR__MODULE_STATE__UNINSTALLED:
            return SR_MS_UNINSTALLED;
        case SR__MODULE_STATE__IMPORTED:
            return SR_MS_IMPORTED;
        case SR__MODULE_STATE__IMPLEMENTED:
            return SR_MS_IMPLEMENTED;
        default:
            return SR_MS_UNINSTALLED;
    }
}

int
sr_schemas_sr_to_gpb(const sr_schema_t *sr_schemas, const size_t schema_cnt, Sr__Schema ***gpb_schemas)
{
    Sr__Schema **schemas = NULL;
    size_t i = 0, j = 0;
    sr_mem_ctx_t *sr_mem = NULL;
    sr_mem_snapshot_t snapshot = { 0, };
    int rc = SR_ERR_OK;

    CHECK_NULL_ARG2(sr_schemas, gpb_schemas);
    if (0 == schema_cnt) {
        *gpb_schemas = NULL;
        return SR_ERR_OK;
    }

    if (sr_schemas->_sr_mem) {
        sr_mem = sr_schemas->_sr_mem;
        sr_mem_snapshot(sr_mem, &snapshot);
    }

    if (0 < schema_cnt) {
        schemas = sr_calloc(sr_mem, schema_cnt, sizeof(*schemas));
        CHECK_NULL_NOMEM_RETURN(schemas);
    }

    for (i = 0; i < schema_cnt; i++) {
        schemas[i] = sr_calloc(sr_mem, 1, sizeof(**schemas));
        CHECK_NULL_NOMEM_GOTO(schemas[i], rc, cleanup);

        sr__schema__init(schemas[i]);
        if (NULL != sr_schemas[i].module_name) {
            if (sr_mem) {
                schemas[i]->module_name = (char *)sr_schemas[i].module_name;
            } else {
                schemas[i]->module_name = strdup(sr_schemas[i].module_name);
                CHECK_NULL_NOMEM_GOTO(sr_schemas[i].module_name, rc, cleanup);
            }
        }
        if (NULL != sr_schemas[i].ns) {
            if (sr_mem) {
                schemas[i]->ns = (char *)sr_schemas[i].ns;
            } else {
                schemas[i]->ns = strdup(sr_schemas[i].ns);
                CHECK_NULL_NOMEM_GOTO(sr_schemas[i].ns, rc, cleanup);
            }
        }
        if (NULL != sr_schemas[i].prefix) {
            if (sr_mem) {
                schemas[i]->prefix = (char *)sr_schemas[i].prefix;
            } else {
                schemas[i]->prefix = strdup(sr_schemas[i].prefix);
                CHECK_NULL_NOMEM_GOTO(schemas[i]->prefix, rc, cleanup);
            }
        }

        schemas[i]->revision = sr_calloc(sr_mem, 1, sizeof (*schemas[i]->revision));
        CHECK_NULL_NOMEM_GOTO(schemas[i]->revision, rc, cleanup);

        sr__schema_revision__init(schemas[i]->revision);
        if (NULL != sr_schemas[i].revision.revision) {
            if (sr_mem) {
                schemas[i]->revision->revision = (char *)sr_schemas[i].revision.revision;
            } else {
                schemas[i]->revision->revision = strdup(sr_schemas[i].revision.revision);
                CHECK_NULL_NOMEM_GOTO(schemas[i]->revision->revision, rc, cleanup);
            }
        }
        if (NULL != sr_schemas[i].revision.file_path_yang) {
            if (sr_mem) {
                schemas[i]->revision->file_path_yang = (char *)sr_schemas[i].revision.file_path_yang;
            } else {
                schemas[i]->revision->file_path_yang = strdup(sr_schemas[i].revision.file_path_yang);
                CHECK_NULL_NOMEM_GOTO(schemas[i]->revision->file_path_yang, rc, cleanup);
            }
        }
        if (NULL != sr_schemas[i].revision.file_path_yin) {
            if (sr_mem) {
                schemas[i]->revision->file_path_yin = (char *)sr_schemas[i].revision.file_path_yin;
            } else {
                schemas[i]->revision->file_path_yin = strdup(sr_schemas[i].revision.file_path_yin);
                CHECK_NULL_NOMEM_GOTO(schemas[i]->revision->file_path_yin, rc, cleanup);
            }
        }

        if (0 < sr_schemas[i].enabled_feature_cnt) {
            schemas[i]->enabled_features = sr_calloc(sr_mem, sr_schemas[i].enabled_feature_cnt, sizeof(*schemas[i]->enabled_features));
            CHECK_NULL_NOMEM_GOTO(schemas[i]->enabled_features, rc, cleanup);
        }
        for (size_t f = 0; f < sr_schemas[i].enabled_feature_cnt; f++) {
            if (NULL != sr_schemas[i].enabled_features[f]) {
                if (sr_mem) {
                    schemas[i]->enabled_features[f] = sr_schemas[i].enabled_features[f];
                } else {
                    schemas[i]->enabled_features[f] = strdup(sr_schemas[i].enabled_features[f]);
                    CHECK_NULL_NOMEM_GOTO(schemas[i]->enabled_features[f], rc, cleanup);
                }
            }
            schemas[i]->n_enabled_features++;
        }

        if (0 < sr_schemas[i].submodule_count) {
            schemas[i]->submodules = sr_calloc(sr_mem, sr_schemas[i].submodule_count, sizeof(*schemas[i]->submodules));
            CHECK_NULL_NOMEM_GOTO(schemas[i]->submodules, rc, cleanup);
        }
        schemas[i]->n_submodules = sr_schemas[i].submodule_count;

        for (size_t s = 0; s < sr_schemas[i].submodule_count; s++) {
            schemas[i]->submodules[s] = sr_calloc(sr_mem, 1, sizeof (*schemas[i]->submodules[s]));
            CHECK_NULL_NOMEM_GOTO(schemas[i]->submodules[s], rc, cleanup);
            sr__schema_submodule__init(schemas[i]->submodules[s]);
            if (NULL != sr_schemas[i].submodules[s].submodule_name) {
                if (sr_mem) {
                    schemas[i]->submodules[s]->submodule_name = (char *)sr_schemas[i].submodules[s].submodule_name;
                } else {
                    schemas[i]->submodules[s]->submodule_name = strdup(sr_schemas[i].submodules[s].submodule_name);
                    CHECK_NULL_NOMEM_GOTO(schemas[i]->submodules[s]->submodule_name, rc, cleanup);
                }
            }

            schemas[i]->submodules[s]->revision = sr_calloc(sr_mem, 1, sizeof (*schemas[i]->submodules[s]->revision));
            CHECK_NULL_NOMEM_GOTO(schemas[i]->submodules[s]->revision, rc, cleanup);
            sr__schema_revision__init(schemas[i]->submodules[s]->revision);
            if (NULL != sr_schemas[i].submodules[s].revision.revision) {
                if (sr_mem) {
                    schemas[i]->submodules[s]->revision->revision = (char *)sr_schemas[i].submodules[s].revision.revision;
                } else {
                    schemas[i]->submodules[s]->revision->revision = strdup(sr_schemas[i].submodules[s].revision.revision);
                    CHECK_NULL_NOMEM_GOTO(schemas[i]->submodules[s]->revision->revision, rc, cleanup);
                }
            }
            if (NULL != sr_schemas[i].submodules[s].revision.file_path_yang) {
                if (sr_mem) {
                    schemas[i]->submodules[s]->revision->file_path_yang = (char *)sr_schemas[i].submodules[s].revision.file_path_yang;
                } else {
                    schemas[i]->submodules[s]->revision->file_path_yang = strdup(sr_schemas[i].submodules[s].revision.file_path_yang);
                    CHECK_NULL_NOMEM_GOTO(schemas[i]->submodules[s]->revision->file_path_yang, rc, cleanup);
                }
            }
            if (NULL != sr_schemas[i].submodules[s].revision.file_path_yin) {
                if (sr_mem) {
                    schemas[i]->submodules[s]->revision->file_path_yin = (char *)sr_schemas[i].submodules[s].revision.file_path_yin;
                } else {
                    schemas[i]->submodules[s]->revision->file_path_yin = strdup(sr_schemas[i].submodules[s].revision.file_path_yin);
                    CHECK_NULL_NOMEM_GOTO(schemas[i]->submodules[s]->revision->file_path_yin, rc, cleanup);
                }
            }
        }
    }

    *gpb_schemas = schemas;
    return SR_ERR_OK;

cleanup:
    if (NULL == sr_mem) {
        for (j = 0; j < i; j++) {
            sr__schema__free_unpacked(schemas[j], NULL);
        }
        free(schemas);
    } else {
        sr_mem_restore(&snapshot);
    }
    return rc;
}

int
sr_schemas_gpb_to_sr(sr_mem_ctx_t *sr_mem, const Sr__Schema **gpb_schemas, const size_t schema_cnt, sr_schema_t **sr_schemas)
{
    sr_schema_t *schemas = NULL;
    size_t i = 0;
    int rc = SR_ERR_OK;
    sr_mem_snapshot_t snapshot = { 0, };

    CHECK_NULL_ARG2(gpb_schemas, sr_schemas);
    if (0 == schema_cnt) {
        *sr_schemas = NULL;
        return SR_ERR_OK;
    }

    if (sr_mem) {
        sr_mem_snapshot(sr_mem, &snapshot);
    }

    if (0 < schema_cnt) {
        schemas = sr_calloc(sr_mem, schema_cnt, sizeof(*schemas));
        CHECK_NULL_NOMEM_RETURN(schemas);
    }

    for (i = 0; i < schema_cnt; i++) {
        schemas[i]._sr_mem = sr_mem;
    }

    for (i = 0; i < schema_cnt; i++) {
        if (NULL != gpb_schemas[i]->module_name) {
            if (sr_mem) {
                schemas[i].module_name = gpb_schemas[i]->module_name;
            } else {
                schemas[i].module_name = strdup(gpb_schemas[i]->module_name);
                CHECK_NULL_NOMEM_GOTO(schemas[i].module_name, rc, cleanup);
            }
        }
        if (NULL != gpb_schemas[i]->ns) {
            if (sr_mem) {
                schemas[i].ns = gpb_schemas[i]->ns;
            } else {
                schemas[i].ns = strdup(gpb_schemas[i]->ns);
                CHECK_NULL_NOMEM_GOTO(schemas[i].ns, rc, cleanup);
            }
        }
        if (NULL != gpb_schemas[i]->prefix) {
            if (sr_mem) {
                schemas[i].prefix = gpb_schemas[i]->prefix;
            } else {
                schemas[i].prefix = strdup(gpb_schemas[i]->prefix);
                CHECK_NULL_NOMEM_GOTO(schemas[i].prefix, rc, cleanup);
            }
        }

        if (NULL != gpb_schemas[i]->revision->revision) {
            if (sr_mem) {
                schemas[i].revision.revision = gpb_schemas[i]->revision->revision;
            } else {
                schemas[i].revision.revision = strdup(gpb_schemas[i]->revision->revision);
                CHECK_NULL_NOMEM_GOTO(schemas[i].revision.revision, rc, cleanup);
            }
        }
        if (NULL != gpb_schemas[i]->revision->file_path_yang) {
            if (sr_mem) {
                schemas[i].revision.file_path_yang = gpb_schemas[i]->revision->file_path_yang;
            } else {
                schemas[i].revision.file_path_yang = strdup(gpb_schemas[i]->revision->file_path_yang);
                CHECK_NULL_NOMEM_GOTO(schemas[i].revision.file_path_yang, rc, cleanup);
            }
        }
        if (NULL != gpb_schemas[i]->revision->file_path_yin) {
            if (sr_mem) {
                schemas[i].revision.file_path_yin = gpb_schemas[i]->revision->file_path_yin;
            } else {
                schemas[i].revision.file_path_yin = strdup(gpb_schemas[i]->revision->file_path_yin);
                CHECK_NULL_NOMEM_GOTO(schemas[i].revision.file_path_yin, rc, cleanup);
            }
        }

        if (0 < gpb_schemas[i]->n_enabled_features) {
            schemas[i].enabled_features = sr_calloc(sr_mem, gpb_schemas[i]->n_enabled_features, sizeof(*schemas[i].enabled_features));
            CHECK_NULL_NOMEM_GOTO(schemas[i].enabled_features, rc, cleanup);
        }
        for (size_t f = 0; f < gpb_schemas[i]->n_enabled_features; f++){
            if (NULL != gpb_schemas[i]->enabled_features[f]) {
                if (sr_mem) {
                    schemas[i].enabled_features[f] = gpb_schemas[i]->enabled_features[f];
                } else {
                    schemas[i].enabled_features[f] = strdup(gpb_schemas[i]->enabled_features[f]);
                    CHECK_NULL_NOMEM_GOTO(schemas[i].enabled_features[f], rc, cleanup);
                }
            }
            schemas[i].enabled_feature_cnt++;
        }

        if (0 < gpb_schemas[i]->n_submodules) {
            schemas[i].submodules = sr_calloc(sr_mem, gpb_schemas[i]->n_submodules, sizeof(*schemas[i].submodules));
            CHECK_NULL_NOMEM_GOTO(schemas[i].submodules, rc, cleanup);
        }

        for (size_t s = 0; s < gpb_schemas[i]->n_submodules; s++) {
            if (NULL != gpb_schemas[i]->submodules[s]->submodule_name) {
                if (sr_mem) {
                    schemas[i].submodules[s].submodule_name = gpb_schemas[i]->submodules[s]->submodule_name;
                } else {
                    schemas[i].submodules[s].submodule_name = strdup(gpb_schemas[i]->submodules[s]->submodule_name);
                    CHECK_NULL_NOMEM_GOTO(schemas[i].submodules[s].submodule_name, rc, cleanup);
                }
            }

            if (NULL != gpb_schemas[i]->submodules[s]->revision->revision) {
                if (sr_mem) {
                    schemas[i].submodules[s].revision.revision = gpb_schemas[i]->submodules[s]->revision->revision;
                } else {
                    schemas[i].submodules[s].revision.revision = strdup(gpb_schemas[i]->submodules[s]->revision->revision);
                    CHECK_NULL_NOMEM_GOTO(schemas[i].submodules[s].revision.revision, rc, cleanup);
                }
            }
            if (NULL != gpb_schemas[i]->submodules[s]->revision->file_path_yang) {
                if (sr_mem) {
                    schemas[i].submodules[s].revision.file_path_yang = gpb_schemas[i]->submodules[s]->revision->file_path_yang;
                } else {
                    schemas[i].submodules[s].revision.file_path_yang = strdup(gpb_schemas[i]->submodules[s]->revision->file_path_yang);
                    CHECK_NULL_NOMEM_GOTO(schemas[i].submodules[s].revision.file_path_yang, rc, cleanup);
                }
            }
            if (NULL != gpb_schemas[i]->submodules[s]->revision->file_path_yin) {
                if (sr_mem) {
                    schemas[i].submodules[s].revision.file_path_yin = gpb_schemas[i]->submodules[s]->revision->file_path_yin;
                } else {
                    schemas[i].submodules[s].revision.file_path_yin = strdup(gpb_schemas[i]->submodules[s]->revision->file_path_yin);
                    CHECK_NULL_NOMEM_GOTO(schemas[i].submodules[s].revision.file_path_yin, rc, cleanup);
                }
            }
            schemas[i].submodule_count++;
        }
    }

    if (sr_mem && schemas) {
        ++sr_mem->obj_count;
    }
    *sr_schemas = schemas;
    return SR_ERR_OK;

cleanup:
    if (sr_mem) {
        sr_mem_restore(&snapshot);
    } else {
        sr_free_schemas(schemas, schema_cnt);
    }
    return rc;
}

int
sr_gpb_fill_error(const char *error_message, const char *error_path, sr_mem_ctx_t *sr_mem, Sr__Error **gpb_error_p)
{
    Sr__Error *gpb_error = NULL;
    sr_mem_snapshot_t snapshot = { 0, };
    int rc = SR_ERR_OK;

    CHECK_NULL_ARG(gpb_error_p);

    if (sr_mem) {
        sr_mem_snapshot(sr_mem, &snapshot);
    }

    gpb_error = sr_calloc(sr_mem, 1, sizeof(*gpb_error));
    CHECK_NULL_NOMEM_RETURN(gpb_error);

    sr__error__init(gpb_error);
    if (NULL != error_message) {
        sr_mem_edit_string(sr_mem, &gpb_error->message, error_message);
        CHECK_NULL_NOMEM_GOTO(gpb_error->message, rc, cleanup);
    }
    if (NULL != error_path) {
        sr_mem_edit_string(sr_mem, &gpb_error->xpath, error_path);
        CHECK_NULL_NOMEM_GOTO(gpb_error->xpath, rc, cleanup);
    }

    *gpb_error_p = gpb_error;
    return SR_ERR_OK;

cleanup:
    if (sr_mem) {
        sr_mem_restore(&snapshot);
    } else {
        if (NULL != gpb_error) {
            sr__error__free_unpacked(gpb_error, NULL);
        }
    }
    return rc;
}

int
sr_gpb_fill_errors(sr_error_info_t *sr_errors, size_t sr_error_cnt, sr_mem_ctx_t *sr_mem, Sr__Error ***gpb_errors_p,
        size_t *gpb_error_cnt_p)
{
    Sr__Error **gpb_errors = NULL;
    sr_mem_snapshot_t snapshot = { 0, };
    int rc = SR_ERR_OK;

    CHECK_NULL_ARG3(sr_errors, gpb_errors_p, gpb_error_cnt_p);

    if (sr_mem) {
        sr_mem_snapshot(sr_mem, &snapshot);
    }

    gpb_errors = sr_calloc(sr_mem, sr_error_cnt, sizeof(*gpb_errors));
    CHECK_NULL_NOMEM_RETURN(gpb_errors);

    for (size_t i = 0; i < sr_error_cnt; i++) {
        rc = sr_gpb_fill_error(sr_errors[i].message, sr_errors[i].xpath, sr_mem, &gpb_errors[i]);
        if (SR_ERR_OK != rc) {
            if (sr_mem) {
                sr_mem_restore(&snapshot);
            } else {
                for (size_t j = 0; j < i; j++) {
                    sr__error__free_unpacked(gpb_errors[j], NULL);
                }
                free(gpb_errors);
            }
            return rc;
        }
    }

    *gpb_errors_p = gpb_errors;
    *gpb_error_cnt_p = sr_error_cnt;

    return SR_ERR_OK;
}
