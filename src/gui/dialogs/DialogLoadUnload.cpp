#include "DialogLoadUnload.hpp"
#include "gui.hpp" //resource_font
#include "sound.hpp"
#include "i18n.h"
#include "client_response_texts.hpp"
#include "ScreenHandler.hpp"
#include "fonts.hpp"
#include "mmu2_error_converter.h"
#include "filament_sensors_handler.hpp"
#include "img_resources.hpp"
#include "fsm_loadunload_type.hpp"
#include <option/has_side_fsensor.h>
#include <option/has_mmu2.h>

RadioButtonMmuErr::RadioButtonMmuErr(window_t *parent, Rect16 rect)
    : AddSuperWindow<RadioButton>(parent, rect) {}

void RadioButtonMmuErr::windowEvent(EventLock /*has private ctor*/, window_t *sender, GUI_event_t event, void *param) {
    switch (event) {
    case GUI_event_t::CLICK: {
        Response response = Click();
        marlin_client::FSM_response(phase, response);
        break;
    }
    default:
        SuperWindowEvent(sender, event, param);
    }
}

void RadioButtonMmuErr::ChangePhase(PhasesLoadUnload phs) {
    if (phase == phs)
        return;
    phase = phs;
    Change(ClientResponses::GetResponses(phs));
}

void RadioButtonMmuErr::ChangePhase(PhasesLoadUnload phs, PhaseResponses responses) {
    phase = phs;
    Change(responses);
}

/*****************************************************************************/
// clang-format off
static const PhaseTexts ph_txt_reheat        = { BtnResponse::GetText(Response::Reheat),           BtnResponse::GetText(Response::_none), BtnResponse::GetText(Response::_none), BtnResponse::GetText(Response::_none) };
static const PhaseTexts ph_txt_disa          = { BtnResponse::GetText(Response::Filament_removed), BtnResponse::GetText(Response::_none), BtnResponse::GetText(Response::_none), BtnResponse::GetText(Response::_none) };
static const PhaseTexts ph_txt_iscolor       = { BtnResponse::GetText(Response::Yes),              BtnResponse::GetText(Response::No),    BtnResponse::GetText(Response::Retry), BtnResponse::GetText(Response::_none) };
static const PhaseTexts ph_txt_iscolor_purge = { BtnResponse::GetText(Response::Yes),              BtnResponse::GetText(Response::No),    BtnResponse::GetText(Response::_none), BtnResponse::GetText(Response::_none) };
static const PhaseTexts ph_txt_unload        = { BtnResponse::GetText(Response::Unload),           BtnResponse::GetText(Response::_none), BtnResponse::GetText(Response::_none), BtnResponse::GetText(Response::_none) };

static const char *txt_first              = N_("Finishing buffered gcodes.");
static const char *txt_tool               = N_("Changing tool");
static const char *txt_parking            = N_("Parking");
static const char *txt_unparking          = N_("Unparking");
static const char *txt_wait_temp          = N_("Waiting for temperature");
static const char *txt_prep_ram           = N_("Preparing to ram");
static const char *txt_ram                = N_("Ramming");
static const char *txt_unload             = N_("Unloading");
static const char *txt_unload_confirm     = N_("Was filament unload successful?");
static const char *txt_filament_not_in_fs = N_("Please remove filament from filament sensor.");
static const char *txt_manual_unload      = N_("Please open idler and remove filament manually");
static const char *txt_push_fil           = N_("Press CONTINUE and push filament into the extruder.");
static const char *txt_make_sure_inserted = N_("Make sure the filament is inserted through the sensor.");
static const char *txt_inserting          = N_("Inserting");
static const char *txt_is_filament_in_gear= N_("Is filament in extruder gear?");
static const char *txt_ejecting           = N_("Ejecting");
static const char *txt_loading            = N_("Loading to nozzle");
static const char *txt_purging            = N_("Purging");
static const char *txt_is_color           = N_("Is color correct?");
static const char *txt_remove_filament    = N_("Please pull out filament immediately.");
#if HAS_LOADCELL()
static const char *txt_filament_stuck     = N_("The filament seems to be stuck, please unload it and load it again.");
#endif
#if HAS_MMU2()
// MMU-related
static const char *txt_mmu_engag_idler    = N_("Engaging idler");
static const char *txt_mmu_diseng_idler   = N_("Disengaging idler");
static const char *txt_mmu_unload_finda   = N_("Unloading to FINDA");
static const char *txt_mmu_unload_pulley  = N_("Unloading to pulley");
static const char *txt_mmu_feed_finda     = N_("Feeding to FINDA");
static const char *txt_mmu_feed_bondtech  = N_("Feeding to drive gear");
static const char *txt_mmu_feed_nozzle    = N_("Feeding to nozzle");
static const char *txt_mmu_avoid_grind    = N_("Avoiding grind");
static const char *txt_mmu_finish_moves   = N_("Finishing moves");
static const char *txt_mmu_err_internal   = N_("ERR Internal");
static const char *txt_mmu_err_help_fil   = N_("ERR Helping filament");
static const char *txt_mmu_err_tmc        = N_("ERR TMC failed");
static const char *txt_mmu_unload_filament= N_("Unloading filament");
static const char *txt_mmu_load_filament  = N_("Loading filament");
static const char *txt_mmu_select_slot    = N_("Selecting filament slot");
static const char *txt_mmu_prepare_blade  = N_("Preparing blade");
static const char *txt_mmu_push_filament  = N_("Pushing filament");
static const char *txt_mmu_perform_cut    = N_("Performing cut");
static const char *txt_mmu_return_selector= N_("Returning selector");
static const char *txt_mmu_park_selector  = N_("Parking selector");
static const char *txt_mmu_eject_filament = N_("Ejecting filament");
static const char *txt_mmu_retract_finda  = N_("Retracting from FINDA");
static const char *txt_mmu_homing         = N_("Homing");
static const char *txt_mmu_moving_selector= N_("Moving selector");
static const char *txt_mmu_feeding_fsensor= N_("Feeding to fsensor");
static const char *txt_mmu_hw_test_begin  = N_("HW test begin");
static const char *txt_mmu_hw_test_idler  = N_("HW test idler");
static const char *txt_mmu_hw_test_sel    = N_("HW test selector");
static const char *txt_mmu_hw_test_pulley = N_("HW test pulley");
static const char *txt_mmu_hw_test_cleanup= N_("HW test cleanup");
static const char *txt_mmu_hw_test_exec   = N_("HW test exec");
static const char *txt_mmu_hw_test_display= N_("HW test display");
static const char *txt_mmu_errhw_test_fail= N_("ERR HW test failed");

//MMU_ErrWaitForUser, // need to distinguish error states based on prusa-error-codes @@TODO
static const char *txt_mmu_err_wait_user  = N_("Waiting for user input");
#endif

static DialogLoadUnload::States LoadUnloadFactory() {
    DialogLoadUnload::States ret = {
        DialogLoadUnload::State { txt_first,                ClientResponses::GetResponses(PhasesLoadUnload::_first),                        ph_txt_none },
        DialogLoadUnload::State { txt_tool,                 ClientResponses::GetResponses(PhasesLoadUnload::ChangingTool),                  ph_txt_none },
        DialogLoadUnload::State { txt_parking,              ClientResponses::GetResponses(PhasesLoadUnload::Parking_stoppable),             ph_txt_stop },
        DialogLoadUnload::State { txt_parking,              ClientResponses::GetResponses(PhasesLoadUnload::Parking_unstoppable),           ph_txt_none },
        DialogLoadUnload::State { txt_wait_temp,            ClientResponses::GetResponses(PhasesLoadUnload::WaitingTemp_stoppable),         ph_txt_stop },
        DialogLoadUnload::State { txt_wait_temp,            ClientResponses::GetResponses(PhasesLoadUnload::WaitingTemp_unstoppable),       ph_txt_none },
        DialogLoadUnload::State { txt_prep_ram,             ClientResponses::GetResponses(PhasesLoadUnload::PreparingToRam_stoppable),      ph_txt_stop },
        DialogLoadUnload::State { txt_prep_ram,             ClientResponses::GetResponses(PhasesLoadUnload::PreparingToRam_unstoppable),    ph_txt_none },
        DialogLoadUnload::State { txt_ram,                  ClientResponses::GetResponses(PhasesLoadUnload::Ramming_stoppable),             ph_txt_stop },
        DialogLoadUnload::State { txt_ram,                  ClientResponses::GetResponses(PhasesLoadUnload::Ramming_unstoppable),           ph_txt_none },
        DialogLoadUnload::State { txt_unload,               ClientResponses::GetResponses(PhasesLoadUnload::Unloading_stoppable),           ph_txt_stop },
        DialogLoadUnload::State { txt_unload,               ClientResponses::GetResponses(PhasesLoadUnload::Unloading_unstoppable),         ph_txt_none },
        DialogLoadUnload::State { txt_remove_filament,      ClientResponses::GetResponses(PhasesLoadUnload::RemoveFilament),                ph_txt_stop, DialogLoadUnload::phaseAlertSound },
        DialogLoadUnload::State { txt_unload_confirm,       ClientResponses::GetResponses(PhasesLoadUnload::IsFilamentUnloaded),            ph_txt_yesno, DialogLoadUnload::phaseWaitSound },
        DialogLoadUnload::State { txt_filament_not_in_fs,   ClientResponses::GetResponses(PhasesLoadUnload::FilamentNotInFS),               ph_txt_none, DialogLoadUnload::phaseAlertSound},
        DialogLoadUnload::State { txt_manual_unload,        ClientResponses::GetResponses(PhasesLoadUnload::ManualUnload),                  ph_txt_continue, DialogLoadUnload::phaseStopSound },
        DialogLoadUnload::State { txt_push_fil,             ClientResponses::GetResponses(PhasesLoadUnload::UserPush_stoppable),            ph_txt_continue_stop, DialogLoadUnload::phaseAlertSound },
        DialogLoadUnload::State { txt_push_fil,             ClientResponses::GetResponses(PhasesLoadUnload::UserPush_unstoppable),          ph_txt_continue, DialogLoadUnload::phaseAlertSound },
        DialogLoadUnload::State { txt_make_sure_inserted,   ClientResponses::GetResponses(PhasesLoadUnload::MakeSureInserted_stoppable),    ph_txt_stop, DialogLoadUnload::phaseAlertSound },
        DialogLoadUnload::State { txt_make_sure_inserted,   ClientResponses::GetResponses(PhasesLoadUnload::MakeSureInserted_unstoppable),  ph_txt_none, DialogLoadUnload::phaseAlertSound },
        DialogLoadUnload::State { txt_inserting,            ClientResponses::GetResponses(PhasesLoadUnload::Inserting_stoppable),           ph_txt_stop },
        DialogLoadUnload::State { txt_inserting,            ClientResponses::GetResponses(PhasesLoadUnload::Inserting_unstoppable),         ph_txt_none },
        DialogLoadUnload::State { txt_is_filament_in_gear,  ClientResponses::GetResponses(PhasesLoadUnload::IsFilamentInGear),              ph_txt_yesno },
        DialogLoadUnload::State { txt_ejecting,             ClientResponses::GetResponses(PhasesLoadUnload::Ejecting_stoppable),            ph_txt_stop },
        DialogLoadUnload::State { txt_ejecting,             ClientResponses::GetResponses(PhasesLoadUnload::Ejecting_unstoppable),          ph_txt_none },
        DialogLoadUnload::State { txt_loading,              ClientResponses::GetResponses(PhasesLoadUnload::Loading_stoppable),             ph_txt_stop },
        DialogLoadUnload::State { txt_loading,              ClientResponses::GetResponses(PhasesLoadUnload::Loading_unstoppable),           ph_txt_none },
        DialogLoadUnload::State { txt_purging,              ClientResponses::GetResponses(PhasesLoadUnload::Purging_stoppable),             ph_txt_stop },
        DialogLoadUnload::State { txt_purging,              ClientResponses::GetResponses(PhasesLoadUnload::Purging_unstoppable),           ph_txt_none },
        DialogLoadUnload::State { txt_is_color,             ClientResponses::GetResponses(PhasesLoadUnload::IsColor),                       ph_txt_iscolor, DialogLoadUnload::phaseAlertSound },
        DialogLoadUnload::State { txt_is_color,             ClientResponses::GetResponses(PhasesLoadUnload::IsColorPurge),                  ph_txt_iscolor_purge, DialogLoadUnload::phaseAlertSound },
        DialogLoadUnload::State { txt_unparking,            ClientResponses::GetResponses(PhasesLoadUnload::Unparking),                     ph_txt_stop },
#if HAS_LOADCELL()
        DialogLoadUnload::State { txt_filament_stuck,       ClientResponses::GetResponses(PhasesLoadUnload::FilamentStuck),                 ph_txt_unload, DialogLoadUnload::phaseAlertSound },
#endif
#if HAS_MMU2()
        DialogLoadUnload::State { txt_mmu_engag_idler,      ClientResponses::GetResponses(PhasesLoadUnload::MMU_EngagingIdler),     ph_txt_none },
        DialogLoadUnload::State { txt_mmu_diseng_idler,     ClientResponses::GetResponses(PhasesLoadUnload::MMU_DisengagingIdler),  ph_txt_none },
        DialogLoadUnload::State { txt_mmu_unload_finda,     ClientResponses::GetResponses(PhasesLoadUnload::MMU_UnloadingToFinda),  ph_txt_none },
        DialogLoadUnload::State { txt_mmu_unload_pulley,    ClientResponses::GetResponses(PhasesLoadUnload::MMU_UnloadingToPulley), ph_txt_none },
        DialogLoadUnload::State { txt_mmu_feed_finda,       ClientResponses::GetResponses(PhasesLoadUnload::MMU_FeedingToFinda),    ph_txt_none },
        DialogLoadUnload::State { txt_mmu_feed_bondtech,    ClientResponses::GetResponses(PhasesLoadUnload::MMU_FeedingToBondtech), ph_txt_none },
        DialogLoadUnload::State { txt_mmu_feed_nozzle,      ClientResponses::GetResponses(PhasesLoadUnload::MMU_FeedingToNozzle),   ph_txt_none },
        DialogLoadUnload::State { txt_mmu_avoid_grind,      ClientResponses::GetResponses(PhasesLoadUnload::MMU_AvoidingGrind),     ph_txt_none },
        DialogLoadUnload::State { txt_mmu_finish_moves,     ClientResponses::GetResponses(PhasesLoadUnload::MMU_FinishingMoves),    ph_txt_none },
        DialogLoadUnload::State { txt_mmu_diseng_idler,     ClientResponses::GetResponses(PhasesLoadUnload::MMU_ERRDisengagingIdler),ph_txt_none },
        DialogLoadUnload::State { txt_mmu_engag_idler,      ClientResponses::GetResponses(PhasesLoadUnload::MMU_ERREngagingIdler),  ph_txt_none },

        // the one and only MMU Error screen
        DialogLoadUnload::State { txt_mmu_err_wait_user,    ClientResponses::GetResponses(PhasesLoadUnload::MMU_ERRWaitingForUser), ph_txt_none },

        DialogLoadUnload::State { txt_mmu_err_internal,     ClientResponses::GetResponses(PhasesLoadUnload::MMU_ERRInternal),       ph_txt_none },
        DialogLoadUnload::State { txt_mmu_err_help_fil,     ClientResponses::GetResponses(PhasesLoadUnload::MMU_ERRHelpingFilament),ph_txt_none },
        DialogLoadUnload::State { txt_mmu_err_tmc,          ClientResponses::GetResponses(PhasesLoadUnload::MMU_ERRTMCFailed),      ph_txt_none },
        DialogLoadUnload::State { txt_mmu_unload_filament,  ClientResponses::GetResponses(PhasesLoadUnload::MMU_UnloadingFilament), ph_txt_none },
        DialogLoadUnload::State { txt_mmu_load_filament,    ClientResponses::GetResponses(PhasesLoadUnload::MMU_LoadingFilament),   ph_txt_none },
        DialogLoadUnload::State { txt_mmu_select_slot,      ClientResponses::GetResponses(PhasesLoadUnload::MMU_SelectingFilamentSlot),ph_txt_none },
        DialogLoadUnload::State { txt_mmu_prepare_blade,    ClientResponses::GetResponses(PhasesLoadUnload::MMU_PreparingBlade),    ph_txt_none },
        DialogLoadUnload::State { txt_mmu_push_filament,    ClientResponses::GetResponses(PhasesLoadUnload::MMU_PushingFilament),   ph_txt_none },
        DialogLoadUnload::State { txt_mmu_perform_cut,      ClientResponses::GetResponses(PhasesLoadUnload::MMU_PerformingCut),     ph_txt_none },
        DialogLoadUnload::State { txt_mmu_return_selector,  ClientResponses::GetResponses(PhasesLoadUnload::MMU_ReturningSelector), ph_txt_none },
        DialogLoadUnload::State { txt_mmu_park_selector,    ClientResponses::GetResponses(PhasesLoadUnload::MMU_ParkingSelector),   ph_txt_none },
        DialogLoadUnload::State { txt_mmu_eject_filament,   ClientResponses::GetResponses(PhasesLoadUnload::MMU_EjectingFilament),  ph_txt_none },
        DialogLoadUnload::State { txt_mmu_retract_finda,    ClientResponses::GetResponses(PhasesLoadUnload::MMU_RetractingFromFinda),ph_txt_none },
        DialogLoadUnload::State { txt_mmu_homing,           ClientResponses::GetResponses(PhasesLoadUnload::MMU_Homing),            ph_txt_none },
        DialogLoadUnload::State { txt_mmu_moving_selector,  ClientResponses::GetResponses(PhasesLoadUnload::MMU_MovingSelector),    ph_txt_none },
        DialogLoadUnload::State { txt_mmu_feeding_fsensor,  ClientResponses::GetResponses(PhasesLoadUnload::MMU_FeedingToFSensor),  ph_txt_none },
        DialogLoadUnload::State { txt_mmu_hw_test_begin,    ClientResponses::GetResponses(PhasesLoadUnload::MMU_HWTestBegin),       ph_txt_none },
        DialogLoadUnload::State { txt_mmu_hw_test_idler,    ClientResponses::GetResponses(PhasesLoadUnload::MMU_HWTestIdler),       ph_txt_none },
        DialogLoadUnload::State { txt_mmu_hw_test_sel,      ClientResponses::GetResponses(PhasesLoadUnload::MMU_HWTestSelector),    ph_txt_none },
        DialogLoadUnload::State { txt_mmu_hw_test_pulley,   ClientResponses::GetResponses(PhasesLoadUnload::MMU_HWTestPulley),      ph_txt_none },
        DialogLoadUnload::State { txt_mmu_hw_test_cleanup,  ClientResponses::GetResponses(PhasesLoadUnload::MMU_HWTestCleanup),     ph_txt_none },
        DialogLoadUnload::State { txt_mmu_hw_test_exec,     ClientResponses::GetResponses(PhasesLoadUnload::MMU_HWTestExec),        ph_txt_none },
        DialogLoadUnload::State { txt_mmu_hw_test_display,  ClientResponses::GetResponses(PhasesLoadUnload::MMU_HWTestDisplay),     ph_txt_none },
        DialogLoadUnload::State { txt_mmu_errhw_test_fail,  ClientResponses::GetResponses(PhasesLoadUnload::MMU_ErrHwTestFailed),   ph_txt_none },
#endif
    };

    return ret;
}
// clang-format on
/*****************************************************************************/

static constexpr Rect16 mmu_title_rect = { 14, 44, 317, 22 };
static constexpr Rect16 mmu_desc_rect = { 14, 66, 224, 105 };
static constexpr Rect16 mmu_icon_rect = { 263, 73, 59, 72 };
static constexpr Rect16 mmu_link_rect = { 14, 165, 317, 32 }; // 2x font size + a bit of margin
static constexpr Rect16 mmu_qr_rect = { 341, 44, 125, 125 };
static constexpr char error_code_link_format[] = N_("More detail at\nprusa.io/%05u");
namespace {
constexpr size_t color_size { 16 };
constexpr size_t text_height { 21 };
constexpr size_t text_margin { 18 };
constexpr size_t top_of_bottom_part { GuiDefaults::ScreenHeight - GuiDefaults::FooterHeight - GuiDefaults::FramePadding - GuiDefaults::ButtonHeight - 5 };
constexpr Rect16 filament_color_icon_rect { 0, top_of_bottom_part - text_height + (text_height - color_size) / 2, color_size, color_size }; // x needs to be 0, to be set later
constexpr Rect16 filament_type_text_rect { text_margin, top_of_bottom_part - text_height, GuiDefaults::ScreenWidth - 2 * text_margin, 21 };
} // namespace

DialogLoadUnload::DialogLoadUnload(fsm::BaseData data)
    : AddSuperWindow<DialogStateful<PhasesLoadUnload>>(get_name(ProgressSerializerLoadUnload(data.GetData()).mode), LoadUnloadFactory(), has_footer::yes)
    , footer(this
#if FOOTER_ITEMS_PER_LINE__ >= 5
          ,
          footer::Item::nozzle, footer::Item::bed, footer::Item::f_sensor
    #if HAS_MMU2()
          ,
          FSensors_instance().HasMMU() ? footer::Item::finda : footer::Item::none,
          FSensors_instance().HasMMU() ? footer::Item::f_s_value : footer::Item::none
    #elif HAS_SIDE_FSENSOR()
          ,
          footer::Item::f_sensor_side
    #else
          ,
          footer::Item::none
    #endif
#endif
          )
    , radio_for_red_screen(this, GuiDefaults::GetIconnedButtonRect(GetRect()) - Rect16::Top_t(GuiDefaults::FooterHeight))
    , text_link(this, mmu_link_rect, is_multiline::yes, is_closed_on_click_t::no)
    , icon_hand(this, mmu_icon_rect, &img::hand_qr_59x72)
    , filament_type_text(this, filament_type_text_rect, is_multiline::no)
    , filament_color_icon(this, filament_color_icon_rect)
    , qr(this, mmu_qr_rect)
    , mode(ProgressSerializerLoadUnload(data.GetData()).mode) {

    filament_type_text.SetAlignment(Align_t::Center());
    filament_color_icon.SetRoundCorners();
    instance = this;

    text_link.set_font(resource_font(IDR_FNT_SMALL));

    radio_for_red_screen.SetHasIcon();
    radio_for_red_screen.Hide();
    text_link.Hide();
    icon_hand.Hide();
    qr.Hide();

    Change(data);
}

DialogLoadUnload::~DialogLoadUnload() {
    // Dtor only resets the header to black.
    // The header could not be red before the screen opened.
    // And even if it were, this behavior would only cause the header to appear in the wrong color.
    // If this starts to cause any problems, it is possible to send an event from ctor to ask for the status of the header
    // and the header could respond through another event.
    // Or we could just implement a color stack in the header.
    event_conversion_union uni;
    uni.header.layout = layout_color::black;
    Screens::Access()->ScreenEvent(this, GUI_event_t::HEADER_COMMAND, uni.pvoid);
    instance = nullptr;
}

DialogLoadUnload *DialogLoadUnload::instance = nullptr;

// Phase callbacks to play a sound in specific moment at the start/end of
// specified phase
void DialogLoadUnload::phaseAlertSound() {
    Sound_Stop();
    Sound_Play(eSOUND_TYPE::SingleBeep);
}
void DialogLoadUnload::phaseWaitSound() {
    if (instance && (instance->get_mode() == LoadUnloadMode::Change || instance->get_mode() == LoadUnloadMode::FilamentStuck)) { /// this sound should be beeping only for M600 || runout
        Sound_Play(eSOUND_TYPE::WaitingBeep);
    }
}
void DialogLoadUnload::phaseStopSound() { Sound_Stop(); }

static constexpr bool isRedMMU([[maybe_unused]] uint8_t phs) {
#if HAS_MMU2()
    int16_t mmu_err_pos = int16_t(PhasesLoadUnload::MMU_ERRWaitingForUser) - int16_t(PhasesLoadUnload::_first);
    return phs == mmu_err_pos;
#else
    return false;
#endif
}

static constexpr bool isRedFStuck([[maybe_unused]] uint8_t phs) {
#if HAS_LOADCELL()
    int16_t fstuck_err_pos = int16_t(PhasesLoadUnload::FilamentStuck) - int16_t(PhasesLoadUnload::_first);
    return phs == fstuck_err_pos;
#else
    return false;
#endif
}

static constexpr bool isRed(uint8_t phs) {
    return isRedMMU(phs) || isRedFStuck(phs);
}

constexpr static const char title_filament_stuck[] = N_("FILAMENT STUCK");

bool DialogLoadUnload::change(uint8_t phs, fsm::PhaseData data) {
    LoadUnloadMode new_mode = ProgressSerializerLoadUnload(data).mode;
    if (new_mode != mode) {
        mode = new_mode;
        title.SetText(get_name(mode));
    }

#if HAS_MMU2() || HAS_LOADCELL()
    // was black (or uninitialized), is red
    if ((!phase || !isRed(*phase)) && isRed(phs)) {
        SetRedLayout();
        // this dialog does not contain header, so it broadcasts event to all windows
        event_conversion_union uni;
        uni.header.layout = layout_color::red;
        Screens::Access()->ScreenEvent(this, GUI_event_t::HEADER_COMMAND, uni.pvoid);

        title.SetRect(mmu_title_rect);

        progress.Hide();

        label.SetRect(mmu_desc_rect);

        radio_for_red_screen.Show(); // show red screen radio button
        CaptureNormalWindow(radio_for_red_screen); // capture red screen radio button
        radio.Hide(); // hide normal radio button

        text_link.Show();
        icon_hand.Show();
        qr.Show();
    }

    // is red
    if (isRed(phs)) {
        if (!can_change(phs))
            return false;

    #if HAS_MMU2()
        if (isRedMMU(phs)) {
            const MMU2::MMUErrDesc *ptr_desc = fsm::PointerSerializer<MMU2::MMUErrDesc>(data).Get();
            PhaseResponses responses {
                ButtonOperationToResponse(ptr_desc->buttons[0]),
                ButtonOperationToResponse(ptr_desc->buttons[1]),
                ButtonOperationToResponse(ptr_desc->buttons[2])
            };
            radio_for_red_screen.ChangePhase(PhasesLoadUnload::MMU_ERRWaitingForUser, responses);
            red_screen_update(ftrstd::to_underlying(ptr_desc->err_code), ptr_desc->err_title, ptr_desc->err_text);
        }
    #endif
    #if HAS_LOADCELL()
        // here, an "else" would be nice, but there might be printers with MMU and without loadcell in the future...
        if (isRedFStuck(phs)) {
            // An ugly workaround to abuse existing infrastructure - this is not an MMU-related error
            // yet we need to throw a dialog with a QR code and a button.
            // @@TODO once the error makes it into Prusa-Error-Codes, we can remove the ErrDesc from this spot
            static constexpr ErrDesc filamentStuckDesc {
                title_filament_stuck,
                "The filament seems to be stuck, please unload it and load it again.",
                (ErrCode)(ERR_PRINTER_CODE * 1000 + 101),
            };
            // I don't like the fact, that the one-and-only response from FilamentStuck (aka Unload) gets mapped onto the first button)
            // It doesn't look nice ;) ... therefore, some handcrafted ugly alignment is necessary at this spot
            PhaseResponses responses { Response::_none, Response::Unload, Response::_none };
            radio_for_red_screen.ChangePhase(PhasesLoadUnload::FilamentStuck, responses);
            red_screen_update(ftrstd::to_underlying(filamentStuckDesc.err_code), filamentStuckDesc.err_title, filamentStuckDesc.err_text);
        }
    #endif
        phase = phs; // set it directly, do not use super::change(phs, data);

        return true;
    }

    // was red (or uninitialized), is black
    if ((!phase || isRed(*phase)) && !isRed(phs)) {
        title.SetRect(get_title_rect(GetRect()));
        SetBlackLayout();
        // this dialog does not contain header, so it broadcasts event to all windows
        event_conversion_union uni;
        uni.header.layout = layout_color::black;
        Screens::Access()->ScreenEvent(this, GUI_event_t::HEADER_COMMAND, uni.pvoid);

        progress.Show();

        label.SetRect(get_label_rect(GetRect(), has_footer::yes));

        radio.Show(); // show normal radio button
        CaptureNormalWindow(radio); // capture normal radio button
        radio_for_red_screen.Hide(); // hide red screen radio button

        text_link.Hide();
        icon_hand.Hide();
        qr.Hide();
    }
#endif

    // is black
    return super::change(phs, data);
}

void DialogLoadUnload::red_screen_update(uint16_t errCode, const char *errTitle, const char *errDesc) {
    title.SetText(string_view_utf8::MakeRAM((const uint8_t *)errTitle));
    label.SetText(string_view_utf8::MakeRAM((const uint8_t *)errDesc));

    snprintf(error_code_str, sizeof(error_code_str), error_code_link_format, errCode);
    text_link.SetText(string_view_utf8::MakeRAM((const uint8_t *)error_code_str));

    qr.SetQRHeader(errCode);
}

constexpr static const char title_change[] = N_("Changing filament");
constexpr static const char title_load[] = N_("Loading filament");
constexpr static const char title_unload[] = N_("Unloading filament");
constexpr static const char title_purge[] = N_("Purging filament");
constexpr static const char title_test[] = N_("Testing filament");
constexpr static const char title_index_error[] = "Index error"; // intentionally not to be translated

string_view_utf8 DialogLoadUnload::get_name(LoadUnloadMode mode) {
    switch (mode) {
    case LoadUnloadMode::Change:
        return _(title_change);
    case LoadUnloadMode::Load:
        return _(title_load);
    case LoadUnloadMode::Unload:
        return _(title_unload);
    case LoadUnloadMode::Purge:
        return _(title_purge);
    case LoadUnloadMode::FilamentStuck:
        return _(title_filament_stuck);
    case LoadUnloadMode::Test:
        return _(title_test);
    default:
        break;
    }
    return string_view_utf8::MakeCPUFLASH((const uint8_t *)title_index_error);
}

float DialogLoadUnload::deserialize_progress(fsm::PhaseData data) const {
    return ProgressSerializerLoadUnload(data).progress;
}

void DialogLoadUnload::phaseEnter() {
    if (!phase)
        return;
    AddSuperWindow<DialogStateful<PhasesLoadUnload>>::phaseEnter();

    if (mode == LoadUnloadMode::Load) { // Change is currently split into Load/Unload, therefore no need to if (mode == change)
        if (filament::get_type_to_load() != filament::Type::NONE) {
            filament_type_text.Show();
            auto fil_name = filament::get_description(filament::get_type_to_load()).name;
            filament_type_text.SetText(_(fil_name));
            if (filament::get_color_to_load().has_value()) {

                int16_t left_pos = (GuiDefaults::ScreenWidth - (resource_font_size(IDR_FNT_NORMAL).w + 1) * (strlen(fil_name) + 1 + 1) - color_size) / 2; // make the pos to be on the left of the text (+ one added space to the left of the text, + additional one for some reason makes it work )
                auto rect = filament_color_icon_rect + Rect16::X_t { static_cast<int16_t>(left_pos) };

                auto col = filament::get_color_to_load().value();
                filament_color_icon.SetBackColor(to_color_t(col.red, col.green, col.blue));
                filament_color_icon.SetRect(rect);
                filament_color_icon.Show();
            } else {
                filament_color_icon.Hide();
            }
        } else {
            filament_type_text.Hide();
            filament_color_icon.Hide();
        }
    } else {
        filament_color_icon.Hide();
        filament_type_text.Hide();
    }
}
