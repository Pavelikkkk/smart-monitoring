import { HashRouter, Routes, Route } from "react-router-dom";

import DashboardLayout from "./layouts/DashboardLayout";
import Home from "./pages/Home";
import Buildings from "./pages/Buildings";
import Devices from "./pages/Devices";
import Anomalies from "./pages/Anomalies";
import Analytics from "./pages/Analytics";
import DeviceDetails from "./pages/DeviceDetails";
import MainLayout from "./layouts/MainLayout";
import BuildingAnalytics from "./pages/BuildingAnalytics";
import Login from "./pages/Login";
import Register from "./pages/Register";
import AuthLayout from "./layouts/AuthLayout";
import Account from "./pages/Account";
import LockedFeature from "./pages/LockedFeature";
import Upgrade from "./pages/Upgrade";
import Settings from "./pages/Settings";
import Subscription from "./pages/Subscription";
import Rooms from "./pages/Rooms";
import RoomDetails from "./pages/RoomDetails";
import Admin from "./pages/Admin";

export default function App() {
  return (
    <HashRouter>
      <Routes>
        <Route
          path="/"
          element={
            <MainLayout>
              <Home />
            </MainLayout>
          }
        />
        <Route
          path="/subscription"
          element={
            <DashboardLayout>
              <Subscription />
            </DashboardLayout>
          }
        />

        <Route
          path="/login"
          element={
            <AuthLayout>
              <Login />
            </AuthLayout>
          }
        />

        <Route
          path="/rooms"
          element={
            <DashboardLayout>
              <Rooms />
            </DashboardLayout>
          }
        />
        <Route
          path="/rooms/:id"
          element={
            <DashboardLayout>
              <RoomDetails />
            </DashboardLayout>
          }
        />

        <Route
          path="/register"
          element={
            <AuthLayout>
              <Register />
            </AuthLayout>
          }
        />

        <Route
          path="/buildings"
          element={
            <DashboardLayout>
              <Buildings />
            </DashboardLayout>
          }
        />
        <Route
          path="/upgrade"
          element={
            <DashboardLayout>
              <Upgrade />
            </DashboardLayout>
          }
        />

        <Route
          path="/devices"
          element={
            <DashboardLayout>
              <Devices />
            </DashboardLayout>
          }
        />

        <Route
          path="/settings"
          element={
            <DashboardLayout>
              <Settings />
            </DashboardLayout>
          }
        />

        <Route
          path="/anomalies"
          element={
            <DashboardLayout>
              <Anomalies />
            </DashboardLayout>
          }
        />

        <Route
          path="/analytics"
          element={
            <DashboardLayout>
              <Analytics />
            </DashboardLayout>
          }
        />

        <Route
          path="/upgrade"
          element={
            <DashboardLayout>
              <LockedFeature />
            </DashboardLayout>
          }
        />

        <Route
          path="/buildings/:id"
          element={
            <DashboardLayout>
              <BuildingAnalytics />
            </DashboardLayout>
          }
        />

        <Route
          path="/devices/:id"
          element={
            <DashboardLayout>
              <DeviceDetails />
            </DashboardLayout>
          }
        />

        <Route
          path="/account"
          element={
            <DashboardLayout>
              <Account />
            </DashboardLayout>
          }
        />

        <Route
          path="/admin"
          element={
            <DashboardLayout>
              <Admin />
            </DashboardLayout>
          }
        />
      </Routes>
    </HashRouter>
  );
}
