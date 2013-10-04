#!/usr/bin/python
# -*- coding: utf-8 -*-

from fileconf import parse_conf_file

def TR(key):
    from logger import Logger
    originalkey = key
    key.replace(" %s", "")
    #Logger().warning(u"Looking for translation for key '%s' (in '%s')" % (key, SESMANCONF.language))
    trans = SESMANCONF.get(SESMANCONF.language)
    if trans is None:
        Logger().warning(u"No translation available for language '%s', falling back to english" % SESMANCONF.language)
        trans = SESMANCONF.get('en')
    message = trans.get(originalkey)
    if message is None:
        Logger().warning(u"No '%s' translation available for key '%s' using key as message" % (SESMANCONF.language, key))
        message = originalkey
    if originalkey.count(u'%s') != message.count(u'%s'):
        Logger().warning(u"Variable parameters mismatch in '%s' message for key '%s' => '%s'" % (SESMANCONF.language, originalkey, message))
    return message

################################################################################
class SesmanConfig(object):
################################################################################

    # __INIT__
    #===============================================================================
    def __init__(self):
    #===============================================================================
        self.language = 'en'
        self.conf = {
          u'RDP':
            { u'clipboard'         : u'True'
            , u'device_redirection': u'True'
            , u'codec_id'          : u'flv'
            }
        , u'VNC':
            { u'clipboard'         : u'False'
            , u'device_redirection': u'False'
            , u'codec_id'          : u'flv'
            }
        , u'sesman' :
            { u'licence'            : u'localhost'
            , u'sql'                : u'localhost'
            , u'x509_authenticated' : u'localhost'
            , u'record_warning'     : u'True'
            }
        , u'fr':
            { u'login'                  : u"login"
            , u'target'                 : u"compte cible"
            , u'password'               : u"mot de passe"
            , u'diagnostic'             : u"diagnostic"
            , u'connection_closed'      : u"connexion fermée"
            , u'OK'                     : u"OK"
            , u'cancel'                 : u"Abandon"
            , u'help'                   : u"Aide"
            , u'close'                  : u"Fermer"
            , u'refused'                : u"Refuser"
            , u'username'               : u"Nom de l'utilisateur"
            , u'password_expire'        : u"Votre mot de passe va bientôt expirer. Veuillez le changer."
            , u'session_closed_at %s'   : u"Votre session sera fermée à %s."
            , u'too_many_connection'    : u"Trop de connexions (par rapport à votre licence)"
            , u'license_blocker'        : u"Connexion refusée (par rapport à votre licence)"
            , u'header_error'           : u"Erreur dans les données d'entêtes reçus du proxy"
            , u'unexpected_error'       : u"Erreur inattendue"
            , u'unexpected_error_key %s': u"Erreur inattendue sur la clef %s"
            , u'auth_failed %s'         : u"Echec de l'authentification pour l'utilisateur %s"
            , u'auth_failed_wab %s'     : u"Echec de l'authentification pour l'utilisateur du wab %s"
            , u'error_recording_path %s': u"Impossible d'obtenir le répertoire d'enregistrement %s"
            , u'session_recorded'       : u"Attention! Votre session va être enregistrée dans un format électronique."
            , u'failed_fetch_port %s %s': u"Impossible d'obtenir le port '%s' pour %s"
            , u'failed_select_target %s': u"Impossible d'obtenir le protocole cible pour %s"
            , u'valid_authorisation'    : u"Veuillez valider l'autorisation sur votre navigateur"
            , u'message_not_validated'  : u"Message non validé"
            , u'not_display_message %s' : u"Impossible d'afficher le message %s"
            , u'password_not_specified' : u"Mot de passe non spécifié"
            , u'connection_refused'     : u"Connexion refusée"
            , u'authorisation_failed %s': u"Impossible d'obtenir les autorisations pour %s"
            , u'unknown_login %s'       : u"Identifiant ou équipement inconnu pour %s"
            , u'ACL_error %s'           : u"Erreur ACL inattendue %s"
            , u'help_message'           :
                                u"Entrez le nom de la mache cible et du compte de la forme login@serveur<br>"
                               +u"Entrez un nom de compte valide<br>dans la zone de saisie Nom de l'utilisateur.<br>"
                               +u"Entrez le mot de passe dans la zone de saisie mot de passe.<br>"
                               +u"Le nom de l'utilisateur et le mot de passe sont sensible à la case.<br>"
                               +u"Contactez votre administrateur système en cas de problème pour vous connecter."
            , u'forceChangePwd'         : u"Vous devez changer votre mot de passe"
            , u'trans_manager_close_cnx': u"Le gestionnaire de session a coupé la connexion"
            , u'changepassword'         : u"Vous devez changer votre mot de passe<br>"
                                         +u"pour vous connecter."
            }
        , u'en':
            { u'login'                  : u"login"
            , u'target'                 : u"target"
            , u'password'               : u"password"
            , u'diagnostic'             : u"diagnostic"
            , u'connection_closed'      : u"connection closed"
            , u'OK'                     : u"OK"
            , u'cancel'                 : u"Cancel"
            , u'help'                   : u"Help"
            , u'close'                  : u"Close"
            , u'refused'                : u"Refused"
            , u'username'               : u"username"
            , u'password_expire'        : u"Your password will expire soon. Please change it."
            , u'session_closed_at %s'   : u"Your session will close at %s."
            , u'too_many_connection'    : u"Too many simultaneous connections (due to licence limitation)"
            , u'license_blocker'        : u"Connection rejected (due to licence limitation)"
            , u'header_error'           : u"Error receiving header data from RDP proxy"
            , u'unexpected_error'       : u"Unexpected error"
            , u'unexpected_error_key %s': u"Unexpected error %s checking keys"
            , u'auth_failed %s'         : u"Authentication failed for user %s"
            , u'auth_failed_wab %s'     : u"Authentication failed for wab user %s"
            , u'error_recording_path %s': u"Failed to get recording path for %s"
            , u'session_recorded'       : u"Warning! Your remote session will be recorded and kept in electronic format."
            , u'failed_fetch_port %s %s': u"Failed to fetch '%s' port for %s"
            , u'failed_select_target %s': u"Failed to select target protocol for %s"
            , u'valid_authorisation'    : u"Validate your authorisation on your browser"
            , u'message_not_validated'  : u"Message not validated"
            , u'not_display_message'    : u"Unable to display message"
            , u'password_not_specified' : u"password is not specified"
            , u'connection_refused'     : u"Connection refused"
            , u'authorisation_failed %s': u"Failed to get authorisations for %s"
            , u'unknown_login %s'       : u"Unknown login or device for %s"
            , u'ACL_error %s'           : u"Unexpected ACL error for %s"
            , u'help_message'           :
                                 u"Enter target device and login as login@server.<br>"
                                +u"Enter a valid authentication user<br>"
                                +u"in the username edit box.<br>"
                                +u"Enter the password in the password edit box.<br>"
                                +u"Both the username and password are case<br>"
                                +u"sensitive.<br>"
                                +u"Contact your system administrator if you are<br>"
                                +u"experiencing problems logging on."
            , u'forceChangePwd'         : u"You must change your password"
            , u'trans_manager_close_cnx': u"Connection closed by manager"
            , u'changepassword'         : u"You must change your password to login"
            }
        }

        parse_conf_file(self.conf, "/opt/wab/share/sesman/config/sesman.conf")


    def __getitem__(self, para):
        return self.conf[para]

    def get(self, para, default = None):
        return self.conf.get(para, None)

# END CLASS - sesmanConfigUnicode

SESMANCONF = SesmanConfig()

