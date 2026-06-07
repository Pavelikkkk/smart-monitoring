import { HashRouter, Routes, Route } from "react-router-dom";

import DashboardLayout from "./layouts/DashboardLayout";
import Home from "./pages/Home";
import Buildings from "./pages/Buildings";
import Devices from "./pages/Devices";
import Anomalies from "./pages/Anomalies";
import BuildingDetails from "./pages/BuildingDetails";
import DeviceDetails from "./pages/DeviceDetails";
import MainLayout from "./layouts/MainLayout";
import BuildingAnalytics from "./pages/BuildingAnalytics";


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
          path="/buildings"
          element={
            <DashboardLayout>
              <Buildings />
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
          path="/anomalies"
          element={
            <DashboardLayout>
              <Anomalies />
            </DashboardLayout>
          }
        />

        <Route
          path="/buildings/:id"
          element={
            <DashboardLayout>
              <BuildingDetails />
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
          path="/buildings/:id/analytics"
          element={
            <DashboardLayout>
              <BuildingAnalytics />
            </DashboardLayout>
          }
        />

      </Routes>

    </HashRouter>
  );
}